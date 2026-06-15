cmake_minimum_required(VERSION 3.16)
project(NonSegmented)

# Find Geant4 package
find_package(Geant4 REQUIRED)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

option(Na22 "Use Na22 source" OFF)
if(Na22)
add_definitions(-DNa22)
endif(Na22)

option(SCINTBAR "Use Scintillator Bar" OFF)
if(SCINTBAR)
add_definitions(-DSCINTBAR)
endif(SCINTBAR)

option(OPTICAL_PHYSICS "Use Scintillator Bar" OFF)
if(OPTICAL_PHYSICS)
add_definitions(-DOPTICAL_PHYSICS)
endif(OPTICAL_PHYSICS)

find_package(ROOT REQUIRED)

# Include directories
include_directories(${Geant4_INCLUDE_DIRS} include)

#ROOT include directories
include_directories(${ROOT_INCLUDE_DIRS})

# Add source files
file(GLOB SOURCES src/*.cpp)
file(GLOB MAC_FILES *.mac)
file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} NonSegmented.cpp)

# Link with Geant4 libraries
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_executable(outMLSim outMLSim.cpp)
target_link_libraries(outMLSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
