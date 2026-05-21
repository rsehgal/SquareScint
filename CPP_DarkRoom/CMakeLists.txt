cmake_minimum_required(VERSION 3.16)
project(SquareScint)

set(CMAKE_CXX_STANDARD 17)

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
target_link_libraries(TestSet ${ROOT_LIBRARIES}  ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ProcessData ProcessData.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(ProcessData ${ROOT_LIBRARIES}  ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ProcessData2 ProcessData2.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(ProcessData2 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ProcessData3 ProcessData3.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(ProcessData3 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(WaveAnal WaveAnal.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(WaveAnal ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ProcessDataWave ProcessDataWave.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(ProcessDataWave ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonDecay MuonDecay.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonDecay ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(DoubleExp DoubleExp.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(DoubleExp ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(DoubleExp_Diss DoubleExp_Diss.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(DoubleExp_Diss ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonArrival MuonArrival.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonArrival ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonCapture MuonCapture.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonCapture ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonDecay_Diss MuonDecay_Diss.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonDecay_Diss ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonDecay2 MuonDecay2.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonDecay2 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonDecay_Bar MuonDecay_Bar.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(MuonDecay_Bar ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(outMLExp outMLExp.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(outMLExp ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(outMLSim outMLSim.cpp Helpers.cpp Histograms.cpp)
target_link_libraries(outMLSim ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)



##########################
add_executable(PlotPulse PlotPulse.cpp)
target_link_libraries(PlotPulse ${ROOT_LIBRARIES})

add_executable(PlotPulse2 PlotPulse2.cpp)
target_link_libraries(PlotPulse2 ${ROOT_LIBRARIES})

add_executable(GainMatch GainMatch.cpp)
target_link_libraries(GainMatch ROOT::Spectrum ${ROOT_LIBRARIES})

add_executable(CorrelationTest CorrelationTest.cpp)
target_link_libraries(CorrelationTest ROOT::Spectrum ${ROOT_LIBRARIES})

add_executable(writePairs writePairs.cpp)
target_link_libraries(writePairs ${ROOT_LIBRARIES})

add_executable(InterBar InterBar.cpp)
target_link_libraries(InterBar ${ROOT_LIBRARIES})

add_executable(PositionCalib PositionCalib.cpp Helpers.cpp)
target_link_libraries(PositionCalib ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(PositionCalib_sq PositionCalib_sq.cpp Helpers.cpp)
target_link_libraries(PositionCalib_sq ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit )

add_executable(PositionCalib_BothDir PositionCalib_BothDir.cpp Helpers.cpp)
target_link_libraries(PositionCalib_BothDir ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)


add_executable(UniformityCheck UniformityCheck.cpp Helpers.cpp )
target_link_libraries(UniformityCheck ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonFluxVariation MuonFluxVariation.cpp Helpers.cpp)
target_link_libraries(MuonFluxVariation ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonFluxVariation_Slab2 MuonFluxVariation_Slab2.cpp Helpers.cpp)
target_link_libraries(MuonFluxVariation_Slab2 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonDecay_Slab2 MuonDecay_Slab2.cpp Helpers.cpp)
target_link_libraries(MuonDecay_Slab2 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ApplyCalib ApplyCalib.cpp Helpers.cpp )
target_link_libraries(ApplyCalib ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ApplyCalib2D ApplyCalib2D.cpp Helpers.cpp )
target_link_libraries(ApplyCalib2D ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(PositionCalib2D PositionCalib2D.cpp Helpers.cpp )
target_link_libraries(PositionCalib2D ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(AccidentalCoincidences AccidentalCoincidences.cpp Helpers.cpp )
target_link_libraries(AccidentalCoincidences ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonFluxVariation_V2 MuonFluxVariation_V2.cpp Helpers.cpp)
target_link_libraries(MuonFluxVariation_V2 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonFluxVariation_V3 MuonFluxVariation_V3.cpp Helpers.cpp)
target_link_libraries(MuonFluxVariation_V3 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MuonFluxVariation_V4 MuonFluxVariation_V4.cpp Helpers.cpp)
target_link_libraries(MuonFluxVariation_V4 ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(TestSensorData TestSensorData.cpp Helpers.cpp)
target_link_libraries(TestSensorData ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(ReadWeatherData ReadWeatherData.cpp Helpers.cpp)
target_link_libraries(ReadWeatherData ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

add_executable(MergeCompassFiles MergeCompassFiles.cpp Helpers.cpp)
target_link_libraries(MergeCompassFiles ${ROOT_LIBRARIES} ROOT::MathCore ROOT::Matrix ROOT::Minuit)

