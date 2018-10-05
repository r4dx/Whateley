cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME eeagl)
project(${PROJECT_NAME})

add_subdirectory(lib/googletest-master)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(eeagl_lib)

add_executable(eeagl src/main.cc)
target_compile_features(eeagl PRIVATE cxx_lambda_init_captures)
include_directories(./lib/cereal-1.2.2/include)

target_sources(eeagl_lib
PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/exec/execution.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/exec/execution.cc 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory_dump.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory_address.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory_address.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/memory_dump.cc 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/command/command.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/command/structure.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/command/structure.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/command/factory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/lang/command/factory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/memory/serialization.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/simulation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vm/simulation.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/generator/generator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/util/random.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/generator/generator.cc
)

target_include_directories(eeagl_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(eeagl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

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


enable_testing()
add_executable(runUnitTests 
	tests/generator/generator_test.cc
	tests/vm/lang/command/structure_test.cc
    tests/vm/memory/memory_test.cc
	tests/vm/memory/memory_address_test.cc
    tests/vm/memory/memory_dump_test.cc 
    tests/vm/memory/memory_dump_test.h
    tests/vm/simulation_test.cc
	tests/vm/exec/execution_test.cc
	tests/util/random_test.cc)

target_link_libraries(runUnitTests gtest gtest_main)
target_link_libraries(runUnitTests eeagl_lib)
target_include_directories(runUnitTests 
PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

set_property(TARGET eeagl_lib PROPERTY CXX_STANDARD 17)
set_property(TARGET eeagl_lib PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET eeagl PROPERTY CXX_STANDARD 17)
set_property(TARGET eeagl PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET runUnitTests PROPERTY CXX_STANDARD 17)
set_property(TARGET runUnitTests PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(tests runUnitTests)