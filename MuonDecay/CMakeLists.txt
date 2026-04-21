cmake_minimum_required(VERSION 3.16)
project(MuonDecay)

# Find Geant4 package
find_package(Geant4 REQUIRED)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Include directories
include_directories(${Geant4_INCLUDE_DIRS} include)

# Add source files
file(GLOB SOURCES src/*.cpp)
file(GLOB MAC_FILES *.mac)
file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} MuonDecay.cpp)

# Link with Geant4 libraries
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
