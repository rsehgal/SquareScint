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

option(USE_CRY "Use cry to generate muons" OFF)
if(USE_CRY)
add_definitions(-DUSE_CRY)
add_subdirectory(cry)
include_directories(cry)
include_directories(cry/Cry/inc)
link_directories(cry)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})

endif(USE_CRY)

# Include directories
include_directories(${Geant4_INCLUDE_DIRS} include)

# Add source files
file(GLOB SOURCES src/*.cpp)
file(GLOB MAC_FILES *.mac)
file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} MuonDecay.cpp)

# Link with Geant4 libraries
if(USE_CRY)
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} cryInterface)
else()
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} )
endif(USE_CRY)

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
