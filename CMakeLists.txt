cmake_minimum_required(VERSION 3.21)

project(HAMT VERSION 4.0 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL) 
	if (APPLE)
		set(CMAKE_OSX_DEPLOYMENT_TARGET "13.3")
	endif() 
	
	add_subdirectory(examples)
  add_subdirectory(tests)
endif()

add_library(hamt INTERFACE)
target_include_directories(hamt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_features(hamt INTERFACE cxx_std_20)

