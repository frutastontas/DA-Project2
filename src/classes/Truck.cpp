#include "Truck.h"

Truck::Truck(int maxCapacity, int palletsCapacity)
    : maxCapacity(maxCapacity), palletsCapacity(palletsCapacity) {}

int Truck::getMaxCapacity() const {
    return maxCapacity;
}

int Truck::getPalletsCapacity() const {
  return palletsCapacity;
}

