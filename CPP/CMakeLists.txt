cmake_minimum_required(VERSION 3.16)
project(SquareScint)

#ROOT
find_package(ROOT REQUIRED)
#message(STATUS ${ROOT_INCLUDE_DIRS})
#message(STATUS ${ROOT_LIBRARIES})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(.)
include_directories(..)

# Add source files
#file(GLOB SOURCES src/*.cpp)
#file(GLOB MAC_FILES *.mac)
#file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
#add_executable(${PROJECT_NAME} ${SOURCES} NaI.cpp)
add_executable(main main.cpp)

# Link with Geant4 libraries
target_link_libraries(main ${ROOT_LIBRARIES})

add_executable(compare compare.cpp)
target_link_libraries(compare ${ROOT_LIBRARIES})
