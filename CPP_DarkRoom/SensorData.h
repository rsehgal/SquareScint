/*
**	Filename : Demo.h
**	2026-04-06
**	username : rsehgal
*/
#ifndef Demo_h
#define Demo_h

#include <algorithm>
#include <numeric>
#include <vector>

struct SensorData {
  double pressure;
  double temp;
  int timestamp;

  SensorData(double pres, double tmp, int ts)
      : pressure(pres), temp(tmp), timestamp(ts) {}
};
template <typename T>
inline constexpr auto get_hourly_time = [](auto &start, const auto &end,
                                               int window) {
  auto limit = start->timestamp + window;

  auto it = std::upper_bound(start, end, limit, [](const auto &val, T &obj) {
    return val < obj.timestamp;
  });

  double sum = std::accumulate(
      start, it, 0., [](double &val, T &obj) { return val + obj.timestamp; });

  int count = std::distance(start, it);

  double mean_time = (count > 0) ? sum / count : 0.;

  start = it;

  return mean_time;
};

template <typename T>
inline constexpr auto get_time_vec =
    [](auto &start, const auto &end, int window) {
      std::vector<double> timeVec;
      while (start != end) {
       timeVec.push_back(get_hourly_time<T>(start, end, window));
      }
      return timeVec;
    };

template <typename T>
inline constexpr auto get_hourly_pressure = [](auto &start, const auto &end,
                                               int window) {
  auto limit = start->timestamp + window;

  auto it = std::upper_bound(start, end, limit, [](const auto &val, T &obj) {
    return val < obj.timestamp;
  });

  double sum = std::accumulate(
      start, it, 0., [](double &val, T &obj) { return val + obj.pressure; });

  int count = std::distance(start, it);

  double mean_press = (count > 0) ? sum / count : 0.;

  start = it;

  return mean_press;
};

template <typename T>
inline constexpr auto get_pressure_vec =
    [](auto &start, const auto &end, int window) {
      std::vector<double> pressureVec;
      while (start != end) {
        pressureVec.push_back(get_hourly_pressure<T>(start, end, window));
      }
      return pressureVec;
    };
#endif
