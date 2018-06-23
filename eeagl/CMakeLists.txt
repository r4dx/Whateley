cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME eeagl)
project(${PROJECT_NAME})

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(eeagl_lib)

add_executable(eeagl src/main.cpp)
target_compile_features(eeagl PRIVATE cxx_lambda_init_captures)

target_sources(eeagl_lib
PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/command.h
)

target_link_libraries(eeagl eeagl_lib)

# Tests

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

add_subdirectory(lib/googletest-master)
enable_testing()
add_executable(runUnitTests tests/vm/memory_test.cpp)
target_link_libraries(runUnitTests gtest gtest_main)
target_link_libraries(runUnitTests eeagl_lib)

set_property(TARGET eeagl_lib PROPERTY CXX_STANDARD 17)
set_property(TARGET eeagl_lib PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET eeagl PROPERTY CXX_STANDARD 17)
set_property(TARGET eeagl PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 17)
set_property(TARGET runUnitTests PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(tests runUnitTests)