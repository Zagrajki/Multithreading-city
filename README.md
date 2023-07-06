Multithreaded city simulation using the pdcurses library. Cars move on an ASCII map. Cars move along their designated routes. If the car runs out of fuel, it drives to a gas station. The gas station has 3 places to refuel the car. When the car has finished refueling, it returns to the designated route. Each car is a separate thread. Cars may not be at the same intersection or use the same filling station at the same time.