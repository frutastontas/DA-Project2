
#include "Solver.h"
#include <vector>

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
    int upperBound = currentWeight;

    for (int i = index; i < pallets.size(); i++) {
        if (currentWeight + pallets[i].getWeight() <= truck.getMaxCapacity()) {
            upperBound += pallets[i].getProfit();
            currentWeight += pallets[i].getWeight();
        }else {
            int remainingWeight = truck.getMaxCapacity() - currentWeight;
            upperBound += (remainingWeight / pallets[i].getWeight()) * pallets[i].getProfit() ;
            break;
        }
    }
    return upperBound;
}


void solveCase1(Truck *truck, Pallet* pallet){

}


