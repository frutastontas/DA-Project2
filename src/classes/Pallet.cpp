#include "Pallet.h"

Pallet::Pallet(int id,int weight, int profit) : id(id),
           weight(weight), profit(profit), ratio(static_cast<double>(profit) / weight) {}

int Pallet::getId()    const{
  return id;
}

int Pallet::getWeight() const {
    return weight;
}

int Pallet::getProfit() const {
    return profit;
}

double Pallet::getRatio() const {
  return ratio;
}