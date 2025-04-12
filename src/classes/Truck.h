#ifndef TRUCK_HPP
#define TRUCK_HPP

class Truck {
private:
    int maxCapacity;
    int palletsCapacity;

public:
    Truck(int maxCapacity, int palletsCapacity);
    int getMaxCapacity() const;
    int getPalletsCapacity() const;
};

#endif // TRUCK_HPP