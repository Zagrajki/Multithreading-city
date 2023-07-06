#ifndef STATION_HPP
#define STATION_HPP
#include "common.hpp"
#include <curses.h>

class Station {
	atomic<bool> idleness = true;
	int fuel = 100;
public:
	void set_idleness(bool state) {
		idleness = state;
	}
	int get_fuel() {
		int temp = fuel;
		fuel = 0;
		return temp;
	}
	int check_fuel() {
		return fuel;
	}
	void load() {
		while (!ending) {
			//refresh();
			if (idleness && fuel < 1000) {
				std::this_thread::sleep_for(timespan_station_filling);
				fuel += 100;
			}
		}
	}
};
Station stations[pumps];
#endif
