
#ifndef SOLVER_H
#define SOLVER_H

#include "classes/Truck.h"
#include <thread>
#include <chrono>
#include "classes/Pallet.h"
#include <iostream>
#include <vector>

int solveCase1(Truck truck, const std::vector<Pallet>& pallets);

int solveCase2(Truck truck,  std::vector<Pallet> &pallets);

int solveDP(const std::vector<Pallet>& pallets, const Truck& truck);

int KnapsackApproximation(const Truck& truck,const std::vector<Pallet>& pallets);

int solveWithILP(const Truck& truck, const std::vector<Pallet>& pallets);
#endif //SOLVER_H
