cmake_minimum_required(VERSION 3.5)
project(CRY C CXX)

enable_language(CXX)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} _build_type)

# Setting the C++ compiler to use
#set("${CMAKE_CXX_COMPILER_ID}" TRUE CACHE STRING "C++ Compiler")
#if (NOT GNU AND NOT Clang AND NOT Intel)
#  message(WARNING "Unsupported compiler. Build will likely fail.")
#endif()

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

option(USE_ROOT "Enable ROOT to create histograms" ON)
if(USE_ROOT)
add_definitions(-DUSE_ROOT)
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
endif(USE_ROOT)

#add_subdirectory(Cry)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Cry/inc)
file(GLOB headersCryInterface ${headersCryInterface} ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/Cry/inc/*.h})
file(GLOB sourcesCryInterface ${sourcesCryInterface} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Cry/src/*.cc)

add_library(cryInterface SHARED ${sourcesCryInterface})

add_executable(Test_Cry ${CMAKE_CURRENT_SOURCE_DIR}/test/Test_Cry.cpp)
add_executable(testDistribution ${CMAKE_CURRENT_SOURCE_DIR}/test/testDistribution.cpp)

link_directories(.)
if(USE_ROOT)
target_link_libraries(Test_Cry cryInterface ${ROOT_LIBRARIES})
target_link_libraries(testDistribution cryInterface ${ROOT_LIBRARIES})
else()
target_link_libraries(Test_Cry cryInterface)
endif(USE_ROOT)

