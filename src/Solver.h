
#ifndef SOLVER_H
#define SOLVER_H

#include "classes/Truck.h"
#include <thread>
#include <chrono>
#include "classes/Pallet.h"
#include <iostream>
#include <vector>

void solveCase1(Truck truck, const std::vector<Pallet>& pallets);

void solveCase2(Truck truck,  std::vector<Pallet> &pallets);

void solveDP(const std::vector<Pallet>& pallets, const Truck& truck);

void KnapsackApproximation(const Truck& truck, std::vector<Pallet>& pallets);
#endif //SOLVER_H
