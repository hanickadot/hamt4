find_package(Catch2 REQUIRED)

add_executable(hamt_tests bitmap.cpp pieces.cpp)
add_coverage(hamt_tests)
target_link_libraries(hamt_tests hamt Catch2::Catch2WithMain)

if (NOT test)
	add_custom_target(test hamt_tests)
endif()
