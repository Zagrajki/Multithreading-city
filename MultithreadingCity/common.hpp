#ifndef COMMON_HPP
#define COMMON_HPP
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
using namespace std;

constexpr int max_fuel = 200;
constexpr int n_crossings = 16;
constexpr int pumps = 3;
constexpr int num_of_cars = 10;//(rand() % 8) + 2;
bool ending = false;
//atomic<bool> refreshable[num_of_cars] = { true };
std::chrono::milliseconds timespan_refresh(40);
std::chrono::milliseconds timespan_sleeping(100);
std::chrono::milliseconds timespan_crossing(900);
std::chrono::milliseconds timespan_station_filling(500);
std::chrono::milliseconds timespan_refueling(500);
struct Coord {
	int x;
	int y;
};
Coord crossings[] = {
	{1, 1}, {1, 14}, {1, 41}, {1, 54},
	{7, 1}, {7, 14}, {7, 41}, {7, 54},
	{13, 1}, {13, 14}, {13, 41}, {13, 54},
	{19, 1}, {19, 14}, {19, 41}, {19, 54} };
mutex mtx[n_crossings];
bool occupancy[n_crossings] = { false };
//atomic<bool> occupancy[18] = { false };
condition_variable cv[n_crossings];

//station
mutex mtx_station;
mutex mtxs_station[num_of_cars];
bool occupancy_station[pumps] = { false };
atomic<bool> occupancy_station_atomic[pumps] = { false };
condition_variable cv_station;
condition_variable cvs_station[num_of_cars];

//refresh - cars
mutex mtxs_refr[num_of_cars];
bool occupancy_refr[num_of_cars] = { false };
condition_variable cvs_refr[num_of_cars];

//refresh - core
mutex mtx_refreshing;
bool refreshing = false;
condition_variable cv_refreshing;

atomic<bool> mtx_refreshing1 = false;

#endif
