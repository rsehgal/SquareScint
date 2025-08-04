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
target_link_libraries(main ${ROOT_LIBRARIES})

add_executable(compare compare.cpp)
target_link_libraries(compare ${ROOT_LIBRARIES})

add_executable(subtract subtract.cpp)
target_link_libraries(subtract ${ROOT_LIBRARIES})

add_executable(TestSet TestSet.cpp Helpers.cpp)
target_link_libraries(TestSet ${ROOT_LIBRARIES})

add_executable(ProcessData ProcessData.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(ProcessData ${ROOT_LIBRARIES})
