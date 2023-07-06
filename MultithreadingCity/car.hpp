#ifndef CAR_HPP
#define CAR_HPP
#include "station.hpp"
#include <curses.h>
#include <vector>
#include <string>

class Car {
	vector<int> my_path;
	Coord actual_position;
	int destination;
	int dest;
	int fuel;
	char ud;
	char lr;
	int my_number;
	int length_of_journey;
	bool is_fuel = true;
	//mutex mtxs_station[my_number];
	//condition_variable cvs_station[my_number];
public:
	Car(int start) {
		length_of_journey = (rand() % (n_crossings - 2)) + 3;
		//mvprintw(0, 57 + start * 3, (to_string(start)).c_str());
		for (int i = 0; i < length_of_journey; i++) {
			my_path.push_back(rand() % n_crossings);
			if (i > 0 && my_path[i] == my_path[i - 1])
			{
				my_path[i] = (my_path[i] + 1) % 16;
			}
			//mvprintw(i + 1, 57 + start * 3, (to_string(my_path[i])).c_str());
		}
		if (my_path[length_of_journey - 1] == my_path[0])
		{
			my_path[length_of_journey - 1] = (my_path[0] + 1) % 16;
			//mvprintw(length_of_journey, 57 + start * 3, (to_string(my_path[length_of_journey - 1])).c_str());
		}
		actual_position = crossings[start];
		if (actual_position.x == crossings[my_path[0]].x && actual_position.y == crossings[my_path[0]].y)
		{
			dest = 1;
			destination = my_path[dest];
		}
		else
		{
			dest = 0;
			destination = my_path[dest];
		}
		fuel = 100 + (rand() % max_fuel);
		if (crossings[destination].x < actual_position.x)
		{
			ud = -1;
			actual_position.y++;
		}
		else if (crossings[destination].x == actual_position.x)
		{
			ud = 0;
		}
		else
		{
			ud = 1;
			actual_position.x++;
		}
		if (crossings[destination].y < actual_position.y)
		{
			lr = -1;
		}
		else if (crossings[destination].y == actual_position.y)
		{
			lr = 0;
		}
		else
		{
			lr = 1;
			if (ud == 0)
			{
				actual_position.x++;
				actual_position.y++;
			}
		}
		my_number = start;
		//mvprintw(actual_position.x, actual_position.y, (to_string(start)+ to_string(ud)+ to_string(lr)).c_str());
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
		refresh();
	}

	void redraw() {
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
	}

	void city();

	void look_for_my_clones() {
		if (char(mvinch(actual_position.x - 1, actual_position.y)) == *to_string(my_number).c_str() || char(mvinch(actual_position.x + 1, actual_position.y)) == *to_string(my_number).c_str() ||
			char(mvinch(actual_position.x, actual_position.y - 1)) == *to_string(my_number).c_str() || char(mvinch(actual_position.x, actual_position.y + 1)) == *to_string(my_number).c_str()) {
			city();
		}
	}

	void refueling(int pump) {
		stations[pump].set_idleness(false);
		std::chrono::milliseconds timespan_refueling(stations[pump].check_fuel() * 5);
		std::this_thread::sleep_for(timespan_refueling);
		fuel = stations[pump].get_fuel();
		is_fuel = true;
		//mvprintw(26, 10, ("fuel:"+to_string(fuel)).c_str());
		stations[pump].set_idleness(true);
	}
	void refuel_left(unique_lock<mutex>& ul_c) {
		mvprintw(actual_position.x, actual_position.y, " ");
		actual_position.x = 20;
		actual_position.y = 14;
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
		std::this_thread::sleep_for(timespan_crossing);
		occupancy[13] = false;
		cv[13].notify_one();
		ul_c.unlock();
		mvprintw(actual_position.x, actual_position.y, " ");
		actual_position.x++;
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
		std::this_thread::sleep_for(timespan_sleeping);
		unique_lock<mutex> ul(mtxs_station[my_number]);
		////cv[idx].wait(ul, glambda(occupancy[idx]));
		cvs_station[my_number].wait(ul, [&]() {return (!occupancy_station[0] || !occupancy_station[1] || !occupancy_station[2]); });
		//if (!occupancy_station[0] || !occupancy_station[1] || !occupancy_station[2]) mtxs_station[my_number].unlock();
			if (!occupancy_station[0]) {
				occupancy_station[0] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 14; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(0);

				for (int i = 0; i < 14; i++) {
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
					std::this_thread::sleep_for(timespan_sleeping);
				}
				occupancy_station[0] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
			}
			else if (!occupancy_station[1]) {
				occupancy_station[1] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 14; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(1);

				for (int i = 0; i < 14; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}
				occupancy_station[1] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
			}
			else if (!occupancy_station_atomic[2]) {
				occupancy_station[2] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 14; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(2);

				for (int i = 0; i < 14; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}
				occupancy_station[2] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
		}
		//ul_c.release();
		ud = -1;
		lr = 0;
	}
	void refuel_right(unique_lock<mutex>& ul_c) {
		mvprintw(actual_position.x, actual_position.y, " ");
		actual_position.x = 20;
		actual_position.y = 41;
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
		std::this_thread::sleep_for(timespan_crossing);
		occupancy[14] = false;
		cv[14].notify_one();
		ul_c.unlock();
		mvprintw(actual_position.x, actual_position.y, " ");
		actual_position.x++;
		mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
		std::this_thread::sleep_for(timespan_sleeping);
		unique_lock<mutex> ul(mtxs_station[my_number], try_to_lock);
		////cv[idx].wait(ul, glambda(occupancy[idx]));
		cvs_station[my_number].wait(ul, [&]() {return (!occupancy_station[0] || !occupancy_station[1] || !occupancy_station[2]); });
			if (!occupancy_station[0]) {
				occupancy_station[0] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(0);

				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}
				occupancy_station[0] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
			}
			else if (!occupancy_station[1]) {
				occupancy_station[1] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(1);

				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}
				occupancy_station[1] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
			}
			else if (!occupancy_station[2]) {
				occupancy_station[2] = true;
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x++;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}

				refueling(2);

				for (int i = 0; i < 13; i++) {
					std::this_thread::sleep_for(timespan_sleeping);
					mvprintw(actual_position.x, actual_position.y, " ");
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					look_for_my_clones();
				}
				occupancy_station[2] = false;
				for (int i = 0; i < num_of_cars; i++) {
					cvs_station[i].notify_one();
				}
				ul.unlock();
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
				mvprintw(actual_position.x, actual_position.y, " ");
				actual_position.x--;
				mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
				look_for_my_clones();
				std::this_thread::sleep_for(timespan_sleeping);
		}
		ud = -1;
		lr = 0;
	}

	int last_dest_in = 0;
	bool not_crossing = true;

	void drive()
	{
		unique_lock<mutex> ul_refr(mtxs_refr[my_number]);
		//auto glambda = [](atomic<bool> a) { return a; };
		//int help = 0;
		while (!ending) {
			//if (getch() != 120) continue;
			refresh();
			if (char(mvinch(actual_position.x - 1, actual_position.y)) == *to_string(my_number).c_str() || char(mvinch(actual_position.x + 1, actual_position.y)) == *to_string(my_number).c_str() ||
				char(mvinch(actual_position.x, actual_position.y - 1)) == *to_string(my_number).c_str() || char(mvinch(actual_position.x, actual_position.y + 1)) == *to_string(my_number).c_str()) {
				city();
				continue;
			}
			if (fuel <= 0 && is_fuel) {
				last_dest_in = my_path[dest];
				my_path[dest] = actual_position.y < 29 ? 13 : 14;
				is_fuel = false;
			}
			else {
				fuel -= 1;
			}
			if (ud == -1) {
				if (!(char(mvinch(actual_position.x - 1, actual_position.y + 1)) == '|' || char(mvinch(actual_position.x - 1, actual_position.y + 1)) == '-') ||
					!(char(mvinch(actual_position.x - 1, actual_position.y - 2)) == '|' || char(mvinch(actual_position.x - 1, actual_position.y - 2)) == '-')) {
					int idx = 16;
					for (int i = 0; i < n_crossings; i++) {
						if (crossings[i].x == actual_position.x - 2 && crossings[i].y == actual_position.y - 1) {
							idx = i;
							break;
						}
					}
					if (idx == 16) {
						city();
						continue;
					}
					//if (!occupancy[idx]) {=
					unique_lock<mutex> ul(mtx[idx]);
					////cv[idx].wait(ul, glambda(occupancy[idx]));
					cv[idx].wait(ul, [&]() {return !occupancy[idx]; });
					occupancy[idx] = true;
					cvs_refr[my_number].wait(ul_refr, [&]() { return !refreshing; });
					occupancy_refr[my_number] = true;
					mvprintw(actual_position.x, actual_position.y, " ");
					occupancy_refr[my_number] = false;
					cv_refreshing.notify_one();
					//ul_refr.unlock();
					if (crossings[idx].x > crossings[my_path[dest]].x) {
						ud = -1;
						actual_position.x -= 2;
					}
					else if (crossings[idx].x < crossings[my_path[dest]].x) {
						ud = 1;
						actual_position.x--;
						actual_position.y--;
					}
					else if (crossings[idx].y > crossings[my_path[dest]].y) {
						ud = 0;
						lr = -1;
						actual_position.x -= 2;
						actual_position.y--;
					}
					else if (crossings[idx].y < crossings[my_path[dest]].y) {
						ud = 0;
						lr = 1;
						actual_position.x--;
					}
					else
					{
						dest = (dest + 1) % length_of_journey;
						if (crossings[idx].x > crossings[my_path[dest]].x) {
							ud = -1;
							actual_position.x -= 2;
						}
						else if (crossings[idx].x < crossings[my_path[dest]].x) {
							ud = 1;
							actual_position.x--;
							actual_position.y--;
						}
						else if (crossings[idx].y > crossings[my_path[dest]].y) {
							ud = 0;
							lr = -1;
							actual_position.x -= 2;
							actual_position.y--;
						}
						else if (crossings[idx].y < crossings[my_path[dest]].y) {
							ud = 0;
							lr = 1;
							actual_position.x--;
						}
						else
						{
							exit(my_number);
						}
					}
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_crossing);
					mvprintw(actual_position.x, actual_position.y, " ");
					not_crossing = false;
					occupancy[idx] = false;
					cv[idx].notify_one();
				}
				else
				{
					if (char(mvinch(actual_position.x - 1, actual_position.y)) == ' ') {
					cvs_refr[my_number].wait(ul_refr, [&]() { return !refreshing; });
					occupancy_refr[my_number] = true;
					if (not_crossing) {
						mvprintw(actual_position.x, actual_position.y, " ");
					}
					else {
						not_crossing = true;
					}
					actual_position.x--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_sleeping);
					}
				}
			}
			else if (ud == 1) {
				if (!(char(mvinch(actual_position.x + 1, actual_position.y - 1)) == '|' || char(mvinch(actual_position.x + 1, actual_position.y - 1)) == '-') ||
					!(char(mvinch(actual_position.x + 1, actual_position.y + 2)) == '|' || char(mvinch(actual_position.x + 1, actual_position.y + 2)) == '-')) {
					int idx = 16;
					for (int i = 0; i < n_crossings; i++) {
						if (crossings[i].x == actual_position.x + 1 && crossings[i].y == actual_position.y) {
							idx = i;
							break;
						}
					}
					if (idx == 16) {
						city();
						continue;
					}
					unique_lock<mutex> ul(mtx[idx]);
					cv[idx].wait(ul, [&]() {return !occupancy[idx]; });
					occupancy[idx] = true;
					mvprintw(actual_position.x, actual_position.y, " ");
					if (crossings[idx].x > crossings[my_path[dest]].x) {
						ud = -1;
						actual_position.x++;
						actual_position.y++;
					}
					else if (crossings[idx].x < crossings[my_path[dest]].x) {
						ud = 1;
						actual_position.x += 2;
					}
					else if (crossings[idx].y > crossings[my_path[dest]].y) {
						ud = 0;
						lr = -1;
						actual_position.x++;
					}
					else if (crossings[idx].y < crossings[my_path[dest]].y) {
						ud = 0;
						lr = 1;
						actual_position.x += 2;
						actual_position.y++;
					}
					else
					{
						if (!is_fuel && my_path[dest] == 13) {
							refuel_left(ul);
							my_path[dest] = last_dest_in; 
							continue; 
						}
						else if (!is_fuel && my_path[dest] == 14) {
							refuel_right(ul);
							my_path[dest] = last_dest_in;
							continue;
						}
						else dest = (dest + 1) % length_of_journey;
						if (crossings[idx].x > crossings[my_path[dest]].x) {
							ud = -1;
							actual_position.x++;
							actual_position.y++;
						}
						else if (crossings[idx].x < crossings[my_path[dest]].x) {
							ud = 1;
							actual_position.x += 2;
						}
						else if (crossings[idx].y > crossings[my_path[dest]].y) {
							ud = 0;
							lr = -1;
							actual_position.x++;
						}
						else if (crossings[idx].y < crossings[my_path[dest]].y) {
							ud = 0;
							lr = 1;
							actual_position.x += 2;
							actual_position.y++;
						}
						else
						{
							exit(my_path[dest]);
						}
					}
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_crossing);
					mvprintw(actual_position.x, actual_position.y, " ");
					not_crossing = false;
					occupancy[idx] = false;
					cv[idx].notify_one();
				}
				else
				{
					if (char(mvinch(actual_position.x + 1, actual_position.y)) == ' ') {
					cvs_refr[my_number].wait(ul_refr, [&]() { return !refreshing; });
					occupancy_refr[my_number] = true;
					if (not_crossing) {
						mvprintw(actual_position.x, actual_position.y, " ");
					}
					else {
						not_crossing = true;
					}
					actual_position.x++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_sleeping);
					}
				}
			}
			else if (lr == -1) {
				if (char(mvinch(actual_position.x - 1, actual_position.y - 1)) != '-' || char(mvinch(actual_position.x + 2, actual_position.y - 1)) != '-') {
					int idx = 16;
					for (int i = 0; i < n_crossings; i++) {
						if (crossings[i].x == actual_position.x && crossings[i].y == actual_position.y - 2) {
							idx = i;
							break;
						}
					}
					if (idx == 16) {
						city();
						continue;
					}
					unique_lock<mutex> ul(mtx[idx]);
					cv[idx].wait(ul, [&]() {return !occupancy[idx]; });
					occupancy[idx] = true;
					mvprintw(actual_position.x, actual_position.y, " ");
					if (crossings[idx].x > crossings[my_path[dest]].x) {
						ud = -1;
						actual_position.y--;
					}
					else if (crossings[idx].x < crossings[my_path[dest]].x) {
						ud = 1;
						actual_position.x++;
						actual_position.y -= 2;
					}
					else if (crossings[idx].y > crossings[my_path[dest]].y) {
						ud = 0;
						lr = -1;
						actual_position.y -= 2;
					}
					else if (crossings[idx].y < crossings[my_path[dest]].y) {
						ud = 0;
						lr = 1;
						actual_position.x++;
						actual_position.y--;
					}
					else
					{
						if (!is_fuel && my_path[dest] == 13) {
							refuel_left(ul);
							my_path[dest] = last_dest_in;
							continue;
						}
						else if (!is_fuel && my_path[dest] == 14) {
							refuel_right(ul);
							my_path[dest] = last_dest_in;
							continue;
						}
						else dest = (dest + 1) % length_of_journey;
						if (crossings[idx].x > crossings[my_path[dest]].x) {
							ud = -1;
							actual_position.y--;
						}
						else if (crossings[idx].x < crossings[my_path[dest]].x) {
							ud = 1;
							actual_position.x++;
							actual_position.y -= 2;
						}
						else if (crossings[idx].y > crossings[my_path[dest]].y) {
							ud = 0;
							lr = -1;
							actual_position.y -= 2;
						}
						else if (crossings[idx].y < crossings[my_path[dest]].y) {
							ud = 0;
							lr = 1;
							actual_position.x++;
							actual_position.y--;
						}
						else
						{
							exit(my_number);
						}
					}
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_crossing);
					mvprintw(actual_position.x, actual_position.y, " ");
					not_crossing = false;
					occupancy[idx] = false;
					cv[idx].notify_one();
				}
				else
				{
					if (char(mvinch(actual_position.x, actual_position.y - 1)) == ' ') {
					cvs_refr[my_number].wait(ul_refr, [&]() { return !refreshing; });
					occupancy_refr[my_number] = true;
					if (not_crossing) {
						mvprintw(actual_position.x, actual_position.y, " ");
					}
					else {
						not_crossing = true;
					}
					actual_position.y--;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_sleeping);
					}
				}
			}
			else if (lr == 1) {
				if (char(mvinch(actual_position.x + 1, actual_position.y + 1)) != '-' || char(mvinch(actual_position.x - 2, actual_position.y + 1)) != '-') {
					int idx = 16;
					for (int i = 0; i < n_crossings; i++) {
						if (crossings[i].x == actual_position.x - 1 && crossings[i].y == actual_position.y + 1) {
							idx = i;
							break;
						}
					}
					if (idx == 16) {
						city();
						continue;
					}
					unique_lock<mutex> ul(mtx[idx]);
					cv[idx].wait(ul, [&]() {return !occupancy[idx]; });
					occupancy[idx] = true;
					mvprintw(actual_position.x, actual_position.y, " ");
					if (crossings[idx].x > crossings[my_path[dest]].x) {
						ud = -1;
						actual_position.x--;
						actual_position.y += 2;
					}
					else if (crossings[idx].x < crossings[my_path[dest]].x) {
						ud = 1;
						actual_position.y++;
					}
					else if (crossings[idx].y > crossings[my_path[dest]].y) {
						ud = 0;
						lr = -1;
						actual_position.x--;
						actual_position.y++;
					}
					else if (crossings[idx].y < crossings[my_path[dest]].y) {
						ud = 0;
						lr = 1;
						actual_position.y += 2;
					}
					else
					{
						if (!is_fuel && my_path[dest] == 13) {
							refuel_left(ul);
							my_path[dest] = last_dest_in; 
							continue; 
						}
						else if (!is_fuel && my_path[dest] == 14) {
							refuel_right(ul); 
							my_path[dest] = last_dest_in;
							continue;
						}
						else dest = (dest + 1) % length_of_journey;
						if (crossings[idx].x > crossings[my_path[dest]].x) {
							ud = -1;
							actual_position.x--;
							actual_position.y += 2;
						}
						else if (crossings[idx].x < crossings[my_path[dest]].x) {
							ud = 1;
							actual_position.y++;
						}
						else if (crossings[idx].y > crossings[my_path[dest]].y) {
							ud = 0;
							lr = -1;
							actual_position.x--;
							actual_position.y++;
						}
						else if (crossings[idx].y < crossings[my_path[dest]].y) {
							ud = 0;
							lr = 1;
							actual_position.y += 2;
						}
						else
						{
							exit(my_number);
						}
					}
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_crossing);
					mvprintw(actual_position.x, actual_position.y, " ");
					not_crossing = false;
					occupancy[idx] = false;
					cv[idx].notify_one();
				}
				else
				{
					if (char(mvinch(actual_position.x, actual_position.y + 1)) == ' ') {
					cvs_refr[my_number].wait(ul_refr, [&]() { return !refreshing; });
					occupancy_refr[my_number] = true;
					if (not_crossing) {
						mvprintw(actual_position.x, actual_position.y, " ");
					}
					else {
						not_crossing = true;
					}
					actual_position.y++;
					mvprintw(actual_position.x, actual_position.y, to_string(my_number).c_str());
					std::this_thread::sleep_for(timespan_sleeping);
					}
				}
			}
		}
	}
};
vector<Car> cars;
void Car::city() {
	if (!mtx_refreshing1) {
		mtx_refreshing1 = true;
		clear();
		printw("---------------------------------------------------------\n");
		printw("|                                                       |\n");
		printw("|                                                       |\n");//0, 1, 2, 3
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|                                                       |\n");
		printw("|                                                       |\n");//4, 5, 6, 7
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|                                                       |\n");
		printw("|                                                       |\n");
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  |         |  |                       |  |         |  |\n");
		printw("|  -----------  -------------------------  -----------  |\n");
		printw("|                                                       |\n");
		printw("|                                                       |\n");
		printw("--------------  -------------------------  --------------\n");
		printw("|            |                             |            |\n");
		printw("|            |  ------------^------------  |            |\n");
		printw("|            |                             |            |\n");
		printw("|            |  ------------^------------  |            |\n");
		printw("|            |                             |            |\n");
		printw("|            ---------------^---------------            |\n");
		for (int i = 0; i < cars.size(); i++) {
			cars[i].redraw();
		}
		mtx_refreshing1 = false;
	}
}
#endif
