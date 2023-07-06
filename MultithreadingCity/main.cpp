#include "car.hpp"

int main()
{
	initscr();
	curs_set(0);
	noecho();
	if (!has_colors()) return 0;
	start_color();
	init_pair(1, COLOR_RED, COLOR_GREEN);
	attron(COLOR_PAIR(1));
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
	/*getch();
	for(int i=0; i<30; i++)
		mvprintw(i, 0, to_string(i).c_str());
	for (int i = 0; i < 57; i=i+2)
		mvprintw(0, i, to_string(i).c_str());*/
	getch();
	/////////////////////////
	vector<thread> th;
	nonl();
	for (int i = 0; i < num_of_cars; i++) {
		cars.push_back(Car(i));
	}getch();
	for (int i = 0; i < pumps; i++) {
		th.push_back(thread(&Station::load, &stations[i]));
	}
	for (int i = 0; i < num_of_cars; i++) {
		th.push_back(thread(&Car::drive, &cars[i]));
	}
	while (!ending)
	{
		if (getch() == 32)
			ending = true;
	}
	for (int i = 0; i < pumps + num_of_cars; i++) {
		th[i].join();
	}
	endwin();
	return 0;
}
