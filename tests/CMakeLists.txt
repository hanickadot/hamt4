find_package(Catch2 REQUIRED)

add_executable(hamt_tests bitmap.cpp)
target_link_libraries(hamt_tests hamt Catch2::Catch2WithMain)

add_custom_target(test hamt_tests)