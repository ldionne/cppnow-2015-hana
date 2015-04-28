# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.0)

#=============================================================================
# Setup required packages
#=============================================================================
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


#=============================================================================
# Setup compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
macro(append_cxx_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

append_cxx_flag(HAS_W_FLAG                         -W)
append_cxx_flag(HAS_WALL_FLAG                      -Wall)
append_cxx_flag(HAS_WEXTRA_FLAG                    -Wextra)
append_cxx_flag(HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
append_cxx_flag(HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
append_cxx_flag(HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
append_cxx_flag(HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)
append_cxx_flag(HAS_STDCXX1Y_FLAG                  -std=c++1y)
append_cxx_flag(HAS_PEDANTIC_FLAG                  -pedantic)


#=============================================================================
# Setup the `sampler` executable
#=============================================================================
include_directories(sampler)
add_executable(sampler sampler/sampler.cpp)


#=============================================================================
# Setup code samples
#=============================================================================
add_custom_target(samples)

file(GLOB CODE_SAMPLES code/*.cpp)
foreach(_file IN LISTS CODE_SAMPLES)
    file(RELATIVE_PATH _target ${CMAKE_CURRENT_SOURCE_DIR}/code ${_file})
    string(REPLACE ".cpp" "" _target "${_target}")
    add_executable(sample.${_target} "${_file}")
    add_dependencies(samples sample.${_target})
endforeach()


#=============================================================================
# Setup the index.html target
#=============================================================================
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/index.html
    COMMAND sampler ${CMAKE_CURRENT_SOURCE_DIR}/index.in.html
                    ${CMAKE_CURRENT_SOURCE_DIR}/index.html
                    ${CODE_SAMPLES}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/index.in.html samples sampler
    COMMENT "Generating index.html from index.in.html and source code samples."
)

add_custom_target(index DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/index.html)
