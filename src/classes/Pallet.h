#ifndef PALLET_HPP
#define PALLET_HPP

class Pallet {
private:
    int id;
    int weight;
    int profit;
    double ratio;

public:
    Pallet(int id,int weight, int profit);
    int getWeight() const;
    int getProfit() const;
    double getRatio() const;
    int getId() const;
};

#endif // PALLET_HPP