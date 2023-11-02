cmake_minimum_required (VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ENABLE_EXPORTS TRUE)

#threading
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)

# Profiling
set(PROFILE_ENABLE 0)
set(COVERAGE_ENABLE 0)


if (PROFILE_ENABLE)
  # usage : gprof sim_gaia_main gmon.out > profOutput.txt
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

# Coverage Profiling
if(COVERAGE_ENABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Compile Options
if(EXISTS "/usr/bin/g++-10")
  set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
  message("COMPILER SET TO G++-10")
endif()

# will be removed -fms-extensions
set(CMAKE_CXX_STANDARD 17)
if (INSTALL_MODE STREQUAL "1")
add_compile_options(-Wall -Wextra -fms-extensions -g -ld)
else()
add_compile_options(-Wall -Wextra -fms-extensions -pedantic -Wno-pedantic -Werror -g -ld)
endif()


# Ignore PCL errors in Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wno-gnu-anonymous-struct -Wno-nested-anon-types)
endif()

project (vector_accelerator)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#include(MetisXHelpers.cmake)
#
#metisx_fetch_external(
#  googletest
#  GIT_REPOSITORY https://github.com/google/googletest.git
#  GIT_TAG v1.13.0
#)

#add_compile_definitions(_HW_PLATFORM_P4_=1)
#add_compile_definitions(_HW_PLATFORM_P4_5=2)
#add_compile_definitions(_HW_PLATFORM_P5_=3)
#
#if (NOT _HW_PLATFORM_)
#add_compile_definitions(_HW_PLATFORM_=_HW_PLATFORM_P4_)
#endif()

# metisx_fetch_external(
#   fmt
#   GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#   GIT_TAG 8.1.1
# )


get_filename_component(METISX_SDK_PATH ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
message(STATUS ${METISX_SDK_PATH})

# add_compile_definitions(_SIM_=1)
#add_subdirectory(metisx)
#add_subdirectory(mu)
#add_subdirectory(test)

##Locate GTest
set(GTEST_SRC /usr/src/gtest/src/gtest-all.cc)
include_directories(SYSTEM /usr/src/gtest # <-- path to non-header files from gtest-all.cc
                           /usr/include)  # <-- path to gtest headers

add_library(gtest STATIC ${GTEST_SRC})    # <-- make static library target before main project executable

add_executable(test.e ut_runner.cpp)
target_link_libraries(test.e gtest)