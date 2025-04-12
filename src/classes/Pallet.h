#ifndef PALLET_HPP
#define PALLET_HPP

class Pallet {
private:
    int id;
    int weight;
    int profit;
    float ratio;

public:
    Pallet(int id,int weight, int profit);
    int getWeight() const;
    int getProfit() const;
    int getRatio() const;
    int getId() const;
};

#endif // PALLET_HPP