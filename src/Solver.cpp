
#include "Solver.h"

#include <algorithm>


void backtrack(const std::vector<Pallet>& pallets, const Truck& truck, int index, int currentProfit,
               int currentWeight, int currentPallets, std::vector<int>& currentCombo,
               std::vector<int>& bestCombo, int& bestProfit) {
    if (currentWeight > truck.getMaxCapacity() || currentPallets > truck.getPalletsCapacity()) {
        return;
    }

    if (currentProfit > bestProfit) {
        bestProfit = currentProfit;
        bestCombo = currentCombo;
    }

    if (index == pallets.size()) return;

    // Exclude current pallet
    backtrack(pallets, truck, index+1, currentProfit, currentWeight, currentPallets,
             currentCombo, bestCombo, bestProfit);

    // Include current pallet
    currentCombo.push_back(pallets[index].getId());
    backtrack(pallets, truck, index + 1,
             currentProfit + pallets[index].getProfit(),
             currentWeight + pallets[index].getWeight(),
             currentPallets + 1,
             currentCombo, bestCombo, bestProfit);
    currentCombo.pop_back();
}

bool sortByRatio(const Pallet& pallet1, const Pallet& pallet2) {
    return pallet1.getRatio() > pallet2.getRatio();
}

int computeUpperBound(const std::vector<Pallet>& pallets, const Truck& truck,
                      int index, int currentProfit, int currentWeight) {
    int remainingWeight = truck.getMaxCapacity() - currentWeight;
    double bound = currentProfit;

    for (int i = index; i < pallets.size(); ++i) {
        if (pallets[i].getWeight() <= remainingWeight) {
            remainingWeight -= pallets[i].getWeight();
            bound += pallets[i].getProfit();
        } else {
            // Take fractionally (for upper bound estimation)
            bound += pallets[i].getProfit() * ((double)remainingWeight / pallets[i].getWeight());
            break;
        }
    }

    return static_cast<int>(bound);
}


/**
 * @brief Solves the knapsack-like problem of selecting pallets to maximize profit
 *        using a backtracking approach enhanced with branch and bound.
 *
 * This function is an adaptation of the basic backtracking approach, augmented with
 * branch-and-bound pruning to improve performance by avoiding unnecessary computations.
 *
 * The function explores all combinations of pallets, deciding whether to include or exclude
 * each one based on current state and feasibility constraints (weight and number of pallets).
 * It uses the helper function `computeUpperBound` to estimate the maximum possible profit from
 * the current state onward. If the upper bound is less than or equal to the best known profit,
 * the branch is pruned.
 *
 * @param pallets A vector of available pallets, each with a weight and profit.
 * @param truck A Truck object containing maximum weight and pallet capacity constraints.
 * @param index The current index of the pallet being considered.
 * @param currentProfit The cumulative profit of the current selection of pallets.
 * @param currentWeight The total weight of the current selection.
 * @param currentPallets The number of pallets selected so far.
 * @param currentCombo A vector storing the IDs of pallets in the current selection.
 * @param bestCombo A vector to store the IDs of pallets forming the best (most profitable) selection.
 * @param bestProfit A reference to the best profit found so far.
 */
void backtrackWithBound(const std::vector<Pallet>& pallets, const Truck& truck, int index,
               int currentProfit, int currentWeight, int currentPallets,
               std::vector<int>& currentCombo, std::vector<int>& bestCombo, int& bestProfit) {

    if (currentWeight > truck.getMaxCapacity() || currentPallets > truck.getPalletsCapacity()) {
        return;
    }

    if (currentProfit > bestProfit) {
        bestProfit = currentProfit;
        bestCombo = currentCombo;
    }

    if (index == pallets.size()) return;

    // --- Bounding ---
    int upperBound = computeUpperBound(pallets, truck, index, currentProfit, currentWeight);
    if (upperBound <= bestProfit) return; // Prune branch

    // Exclude current pallet
    backtrackWithBound(pallets, truck, index + 1, currentProfit, currentWeight, currentPallets,
              currentCombo, bestCombo, bestProfit);

    // Include current pallet
    currentCombo.push_back(pallets[index].getId());
    backtrackWithBound(pallets, truck, index + 1,
              currentProfit + pallets[index].getProfit(),
              currentWeight + pallets[index].getWeight(),
              currentPallets + 1,
              currentCombo, bestCombo, bestProfit);
    currentCombo.pop_back(); // backtrack
}



void solveCase1(Truck truck, const std::vector<Pallet>& pallets){
    std::vector<int> bestCombo;
    int bestProfit =0;
    std::vector<int> currentCombo;
    backtrack(pallets, truck, 0, 0, 0, 0, currentCombo, bestCombo, bestProfit);
    std::cout<<bestProfit<<std::endl;
    for (const int pallet : bestCombo) {
        std::cout<<pallet<<std::endl;
    }
}


void solveCase2(Truck truck, std::vector<Pallet>& pallets){
    std::vector<int> bestCombo;
    int bestProfit =0;
    std::vector<int> currentCombo;

    std::sort(pallets.begin(), pallets.end(), sortByRatio);

    backtrackWithBound(pallets, truck, 0, 0, 0, 0, currentCombo, bestCombo, bestProfit);
    std::cout<<bestProfit<<std::endl;
    for (const int pallet : bestCombo) {
        std::cout<<pallet<<std::endl;
    }
}


