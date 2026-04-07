/*
**	Filename : TestDemo.cpp
**	2026-04-06
**	username : rsehgal
*/
#include "SensorData.h"
#include <iostream>

int main(int argc, char *argv[]) {

  std::vector<SensorData> vecOfSensorData;

  vecOfSensorData.emplace_back(1012., 26.5, 1675);
  vecOfSensorData.emplace_back(1015., 26.5, 1725);
  vecOfSensorData.emplace_back(1011., 26.5, 1750);
  vecOfSensorData.emplace_back(1013., 26.5, 1780);
  vecOfSensorData.emplace_back(1012., 26.5, 1895);
  vecOfSensorData.emplace_back(1024., 26.5, 1980);
  vecOfSensorData.emplace_back(1012., 26.5, 2970);

  std::cout << "--------------------------------------" << std::endl;
  {
    auto current_it = vecOfSensorData.begin();
    std::vector<double> pressureVec;
    while (current_it != vecOfSensorData.end()) {
      double average = get_hourly_pressure<SensorData>(
          current_it, vecOfSensorData.end(), 100);
      pressureVec.push_back(average);
      std::cout << "Average : " << average << std::endl;
    }

    std::cout << "--------------------------------------" << std::endl;
    for (const auto val : pressureVec) {
      std::cout << val << " , ";
    }
    std::cout << "\n";
  }
  std::cout << "--------------------------------------" << std::endl;
  {
    auto current_it = vecOfSensorData.begin();
    std::vector<double> pressureVec =
        get_pressure_vec<SensorData>(current_it, vecOfSensorData.end(), 100);
    for (const auto val : pressureVec) {
      std::cout << val << " , ";
    }
    std::cout << "\n";
  }
  return 0;
}
