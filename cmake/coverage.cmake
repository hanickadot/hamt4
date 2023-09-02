option(HAMT_SOURCE_COVERAGE "Enable source code coverage measurements" ON)

set(HAMT_SOURCE_COVERAGE_ENABLED ${HAMT_SOURCE_COVERAGE})
set(HAMT_SOURCE_COVERAGE_MODE "unknown")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(HAMT_SOURCE_COVERAGE_MODE "clang")
endif()


if (HAMT_SOURCE_COVERAGE_ENABLED AND HAMT_SOURCE_COVERAGE_MODE STREQUAL "clang")
	
message(STATUS "Code coverage report is enabled.")

add_compile_options("-fprofile-instr-generate" "-fcoverage-mapping" "-g" "-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=/")
add_link_options("-fprofile-instr-generate" "-fcoverage-mapping")

# this is a trick to use my own assert() instead problematic system's
add_compile_options(-isystem ${CMAKE_CURRENT_SOURCE_DIR}/cmake/polyfill)

function(add_coverage TARGET_NAME)
	set(INDEX_DIR "${CMAKE_BINARY_DIR}/report-coverage-${TARGET_NAME}")
	set(INDEX_HTML "${INDEX_DIR}/index.html")
	
	add_custom_command(OUTPUT ${INDEX_HTML}
		COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}> 
		COMMAND llvm-profdata merge -sparse ${TARGET_NAME}.profraw -o ${TARGET_NAME}.profdata
		COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata "-ignore-filename-regex=\"(tests/.*|.*/assert.hpp)\"" -format html -output-dir ${INDEX_DIR} -show-instantiations=true -show-line-counts -Xdemangler c++filt -Xdemangler -n -show-branches=percent -path-equivalence=/,${CMAKE_SOURCE_DIR} -tab-size=4
		COMMAND cd ${CMAKE_BINARY_DIR} && zip -q -r -9 code-coverage-${TARGET_NAME}.zip report-coverage-${TARGET_NAME}
		MAIN_DEPENDENCY ${TARGET_NAME}
		COMMENT "Running ${TARGET_NAME} for code-coverage report..."
	)
	
	add_custom_target(ccov-${TARGET_NAME} COMMAND open ${INDEX_HTML} DEPENDS ${INDEX_HTML})
	
	if (NOT COMMAND code-coverage-report)
		add_custom_command(OUTPUT code-coverage-report DEPENDS ${INDEX_HTML})
	else()
		add_custom_command(OUTPUT code-coverage-report DEPENDS ${INDEX_HTML} APPEND)
	endif()
	
	if (NOT TARGET coverage)
		add_custom_target(coverage DEPENDS code-coverage-report)
	endif()
endfunction()

else()

message(STATUS "Code coverage report is disabled.")

function(add_coverage TARGET_NAME)
	
endfunction()
	
endif()
