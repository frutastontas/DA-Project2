
#include "Solver.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <algorithm>

/**
 * @brief Recursive backtracking solution to the knapsack problem.
 *
 * Explores all possible subsets of pallets by recursively including/excluding each pallet.
 *
 * Time Complexity: O(2^n) — explores every combination.
 * Space Complexity: O(n) — due to recursion stack and combo storage.
 *
 * @param pallets List of available pallets.
 * @param truck Constraints: max weight and max number of pallets.
 * @param index Current pallet index.
 * @param currentProfit Current profit accumulated.
 * @param currentWeight Current weight accumulated.
 * @param currentPallets Number of pallets used so far.
 * @param currentCombo Pallet IDs in current selection.
 * @param bestCombo Pallet IDs of best selection found.
 * @param bestProfit Best profit found so far.
 */

void backtrack(const std::vector<Pallet>& pallets, const Truck& truck, int index, int currentProfit,
               int currentWeight, int currentPallets, std::vector<int>& currentCombo,
               std::vector<int>& bestCombo, int& bestProfit) {
    if (index == pallets.size()) {
        if (currentWeight <= truck.getMaxCapacity() &&
            currentPallets <= truck.getPalletsCapacity()
            ) {
            if (currentProfit > bestProfit) {
                bestProfit = currentProfit;
                bestCombo = currentCombo;
            } else if (currentProfit == bestProfit) {
                if (currentCombo.size() < bestCombo.size() ||
                    (currentCombo.size() == bestCombo.size() && currentCombo < bestCombo)) {
                    bestCombo = currentCombo;
                    }
            }

            }
        return;
    }

    // Exclude current pallet
    backtrack(pallets, truck, index + 1, currentProfit, currentWeight, currentPallets,
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

bool sortByProfit(const Pallet& pallet1, const Pallet& pallet2) {
    return pallet1.getProfit() > pallet2.getProfit();
}

/**
 * @brief Computes an upper bound on maximum achievable profit from current index.
 *
 * Uses fractional knapsack logic for bounding in branch-and-bound.
 *
 * Time Complexity: O(n log n) — due to sorting the remaining items.
 * Space Complexity: O(n) — for creating the list of remaining pallets.
 *
 * @param pallets Pallet list.
 * @param truck Truck constraints.
 * @param index Current index.
 * @param currentProfit Accumulated profit.
 * @param currentWeight Accumulated weight.
 * @return Estimated upper bound profit.
 */
double computeUpperBound(const std::vector<Pallet>& pallets, const Truck& truck,
                      int index, int currentProfit, int currentWeight) {
    int remainingWeight = truck.getMaxCapacity() - currentWeight;
    double bound = currentProfit;

    std::vector<Pallet> remaining;
    for (int i = index; i < pallets.size(); i++) {
        remaining.push_back(pallets[i]);
    }

    std::sort(remaining.begin(), remaining.end(), sortByRatio);

    for (const auto& pallet : remaining) {
        if (pallet.getWeight() <= remainingWeight) {
            remainingWeight -= pallet.getWeight();
            bound += pallet.getProfit();
        } else {
            bound += pallet.getProfit() * ((double)remainingWeight / pallet.getWeight());
            break;
        }
        if (remainingWeight == 0) break;
    }

    return bound;
}


/**
 * @brief Enhanced backtracking using branch-and-bound to prune infeasible branches.
 *
 * Skips exploring branches that cannot improve current best profit (using computeUpperBound).
 *
 * Time Complexity: O(2^n) in worst case, but significantly pruned in practice.
 * Space Complexity: O(n) — due to recursion and combo storage.
 *
 * @param pallets List of pallets.
 * @param truck Truck constraints.
 * @param index Current pallet index.
 * @param currentProfit Profit so far.
 * @param currentWeight Weight so far.
 * @param currentPallets Count of pallets used.
 * @param currentCombo Pallet selection being explored.
 * @param bestCombo Best solution found so far.
 * @param bestProfit Maximum profit found.
 */
void backtrackWithBound(const std::vector<Pallet>& pallets, const Truck& truck, int index,
               int currentProfit, int currentWeight, int currentPallets,
               std::vector<int>& currentCombo, std::vector<int>& bestCombo, int& bestProfit) {

    if (index == pallets.size()) {
        if (currentWeight <= truck.getMaxCapacity() &&
            currentPallets <= truck.getPalletsCapacity()) {

            if (currentProfit > bestProfit) {
                bestProfit = currentProfit;
                bestCombo = currentCombo;
            } else if (currentProfit == bestProfit) {
                if (currentCombo.size() < bestCombo.size() ||
                    (currentCombo.size() == bestCombo.size() && currentCombo < bestCombo)) {
                    bestCombo = currentCombo;
                    }
            }
            }
        return;
    }

    // --- Bounding ---
    double upperBound = computeUpperBound(pallets, truck, index, currentProfit, currentWeight);
    if (upperBound < static_cast<double>(bestProfit)) return; // Prune branch

    // Exclude current pallet
    backtrackWithBound(pallets, truck, index + 1, currentProfit, currentWeight, currentPallets,
              currentCombo, bestCombo, bestProfit);

    // --- Include current pallet if within constraints ---
    const Pallet& pallet = pallets[index];
    int nextWeight = currentWeight + pallet.getWeight();
    int nextPallets = currentPallets + 1;

    if (nextWeight <= truck.getMaxCapacity() && nextPallets <= truck.getPalletsCapacity()) {
        currentCombo.push_back(pallet.getId());

        backtrackWithBound(pallets, truck, index + 1,
                           currentProfit + pallet.getProfit(),
                           nextWeight,
                           nextPallets,
                           currentCombo, bestCombo, bestProfit);

        currentCombo.pop_back(); // backtrack
    }
}




int solveCase1(Truck truck, const std::vector<Pallet>& pallets){
    std::vector<int> bestCombo;
    int bestProfit =0;
    std::vector<int> currentCombo;
    auto start = std::chrono::high_resolution_clock::now();
    backtrack(pallets, truck, 0, 0, 0, 0, currentCombo, bestCombo, bestProfit);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "IDs of pallets used: ";
    for (const int pallet : bestCombo) {
        std::cout<<pallet<<",";
    }
    std::cout << std::endl;
    std::cout << "Time taken by solveCase1: " << duration.count() << " seconds\n";
    std::cout<<"Best Total Profit "<<bestProfit<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return bestProfit;
}


int solveCase2(Truck truck, std::vector<Pallet>& pallets){
    std::vector<int> bestCombo;
    int bestProfit =0;
    std::vector<int> currentCombo;
    auto start = std::chrono::high_resolution_clock::now();
    backtrackWithBound(pallets, truck, 0, 0, 0, 0, currentCombo, bestCombo, bestProfit);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "IDs of pallets used: ";
    for (const int pallet : bestCombo) {
        std::cout<<pallet<<",";
    }
    std::cout << std::endl;
    std::cout << "Time taken by solveCase2: " << duration.count() << " seconds\n";
    std::cout<< "Best Total Profit "<<bestProfit<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return bestProfit;
}


/**
 * @brief Solves the knapsack problem using dynamic programming.
 *
 * Uses a 2D DP table where dp[w][k] stores the max profit for weight w and k pallets.
 *
 * Time Complexity: O(n * W * K) — n: pallets, W: weight limit, K: pallet count limit.
 * Space Complexity: O(W * K) — space for DP table.
 *
 * @param pallets List of pallets.
 * @param truck Truck capacity constraints.
 */
int solveDP(const std::vector<Pallet>& pallets, const Truck& truck) {
    auto start = std::chrono::high_resolution_clock::now();
    int maxW = truck.getMaxCapacity();
    int maxP = truck.getPalletsCapacity();

    std::vector<std::vector<int>> dp(maxW + 1, std::vector<int>(maxP + 1, 0));
    std::vector<std::vector<std::vector<int>>> chosen(maxW + 1, std::vector<std::vector<int>>(maxP + 1));

    int bestProfit = 0;
    int bestW = 0, bestK = 0;

    for (const Pallet& p : pallets) {
        for (int w = maxW; w >= p.getWeight(); --w) {
            for (int k = maxP; k >= 1; --k) {
                int prevProfit = dp[w - p.getWeight()][k - 1];
                int newProfit = prevProfit + p.getProfit();

                if (newProfit > dp[w][k]) {
                    dp[w][k] = newProfit;
                    chosen[w][k] = chosen[w - p.getWeight()][k - 1];
                    chosen[w][k].push_back(p.getId());
                }
                else if (newProfit == dp[w][k]) {
                    std::vector<int> newCombo = chosen[w - p.getWeight()][k - 1];
                    newCombo.push_back(p.getId());

                    // Tie-breaking: prefer fewer pallets, then lexicographically smaller combo
                    if (newCombo.size() < chosen[w][k].size() ||
                        (newCombo.size() == chosen[w][k].size() && newCombo < chosen[w][k])) {
                        chosen[w][k] = newCombo;
                    }
                }

                // Update best solution with tie-breaking
                if (dp[w][k] > bestProfit ||
                    (dp[w][k] == bestProfit &&
                     (chosen[w][k].size() < chosen[bestW][bestK].size() ||
                      (chosen[w][k].size() == chosen[bestW][bestK].size() &&
                       chosen[w][k] < chosen[bestW][bestK])))) {
                    bestProfit = dp[w][k];
                    bestW = w;
                    bestK = k;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Best profit: " << bestProfit << std::endl;
    std::cout << "Pallets used (IDs): ";
    for (int id : chosen[bestW][bestK]) {
        std::cout << id << " ";
    }
    std::cout << "\n";
    std::cout << "Time taken by solveDP: " << duration.count() << " seconds\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return bestProfit;
}


/**
 * @brief Greedy heuristic for approximate knapsack solution.
 *
 * Selects pallets while respecting weight and count constraints using a sorted heuristic.
 *
 * Time Complexity: O(n) — iterates once over pallets.
 * Space Complexity: O(1) — aside from output combo vector.
 *
 * @param truck Truck constraints.
 * @param pallets List of pallets (pre-sorted).
 * @param bestCombo Output vector for selected pallet IDs.
 * @return Total profit obtained.
 */

int GreedyKnapsack(const Truck& truck, std::vector<Pallet>& pallets, std::vector<int>& bestCombo) {
    int bestProfit = 0;
    int remainingWeight = truck.getMaxCapacity();
    int currentPallets = 0;

    for (const Pallet& p : pallets) {
        if (currentPallets >= truck.getPalletsCapacity()) break;

        if (p.getWeight() <= remainingWeight) {
            bestProfit += p.getProfit();
            remainingWeight -= p.getWeight();
            currentPallets++;
            bestCombo.push_back(p.getId());
        }
    }
    return bestProfit;
}

/**
 * @brief Runs two greedy heuristics: by profit and by profit/weight ratio.
 *
 * Picks the better result of the two.
 *
 * Time Complexity: O(n log n) — due to sorting twice.
 * Space Complexity: O(n) — due to combo vectors.
 *
 * @param truck Truck capacity constraints.
 * @param pallets List of available pallets.
 */
int KnapsackApproximation(const Truck& truck,const std::vector<Pallet>& pallets) {
    std::vector<int> bestComboProfit;
    std::vector<int> bestComboRatio;

    std::vector<Pallet> copy_pallets = pallets;

    // Sort by profit/weight ratio and run greedy
    std::sort(copy_pallets.begin(), copy_pallets.end(), sortByRatio);
    int ResultRatio = GreedyKnapsack(truck, copy_pallets, bestComboRatio);

    // Sort by profit and run greedy
    std::sort(copy_pallets.begin(), copy_pallets.end(), sortByProfit);
    int ResultProfit = GreedyKnapsack(truck, copy_pallets, bestComboProfit);

    // Decide which result is better
    std::vector<int> bestCombo;
    int bestResult;
    std::string strategy;

    if (ResultProfit >= ResultRatio) {
        bestResult = ResultProfit;
        bestCombo = bestComboProfit;
        strategy = "Profit-based Greedy";
    } else {
        bestResult = ResultRatio;
        bestCombo = bestComboRatio;
        strategy = "Ratio-based Greedy";
    }

    // Print results
    std::cout << "=== Approximation Result ===\n";
    std::cout << "Strategy used: " << strategy << std::endl;
    std::cout << "Total profit: " << bestResult << std::endl;
    std::cout << "Pallets used (IDs): ";
    int totalWeight = 0;
    for (int id : bestCombo) {
        std::cout << id << " ";
    }
    std::cout << "\n";


    std::this_thread::sleep_for(std::chrono::seconds(2));
    return bestResult;
}



/**
 * @brief Solves the 0/1 Knapsack problem using a hybrid approach.
 *
 * For small problem sizes (n <= 30), uses dynamic programming to find the optimal solution.
 * For larger inputs, it applies a greedy heuristic (based on profit-to-weight ratio) followed
 * by a local search improvement via bit-flip neighborhood search.
 *
 * Time Complexity:
 * - DP: O(n * W * K) for small n (<= 30)
 * - Hybrid: O(n log n + n^2) — due to greedy sorting + local search
 *
 * Space Complexity:
 * - DP: O(W * K)
 * - Hybrid: O(n)
 *
 * @param truck The Truck object with capacity constraints.
 * @param pallets List of available pallets (with precomputed ratios).
 * @return Total profit of the best solution found.
 */
int solveHybridKnapsack(const Truck& truck, const std::vector<Pallet>& pallets) {
    const int n = pallets.size();

    // Use DP for small instances
    if (n <= 30) {
        std::cout << "[Hybrid] Using DP for small instance (n = " << n << ")\n";
        return solveDP(pallets, truck);
    }

    std::cout << "[Hybrid] Using Greedy + Local Search for large instance (n = " << n << ")\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<bool> isSelected(n, false);
    std::vector<std::pair<double, int>> ratioIndex;

    for (int i = 0; i < n; ++i)
        ratioIndex.emplace_back(pallets[i].getRatio(), i);

    std::sort(ratioIndex.begin(), ratioIndex.end(), [](auto& a, auto& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second;  // tie-break by ID
    });

    int currProfit = 0, currWeight = 0, currCount = 0;
    std::vector<int> bestCombo;

    for (auto& [_, idx] : ratioIndex) {
        const Pallet& p = pallets[idx];
        if (currWeight + p.getWeight() <= truck.getMaxCapacity() &&
            currCount + 1 <= truck.getPalletsCapacity()) {
            isSelected[idx] = true;
            currProfit += p.getProfit();
            currWeight += p.getWeight();
            currCount++;
            bestCombo.push_back(p.getId());
        }
    }

    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < n; ++i) {
            std::vector<bool> candidate = isSelected;
            candidate[i] = !candidate[i];

            int tempProfit = 0, tempWeight = 0, tempCount = 0;
            std::vector<int> tempCombo;

            for (int j = 0; j < n; ++j) {
                if (candidate[j]) {
                    tempProfit += pallets[j].getProfit();
                    tempWeight += pallets[j].getWeight();
                    tempCount++;
                    tempCombo.push_back(pallets[j].getId());
                }
            }

            if (tempWeight <= truck.getMaxCapacity() && tempCount <= truck.getPalletsCapacity()) {
                bool better = false;
                if (tempProfit > currProfit) better = true;
                else if (tempProfit == currProfit) {
                    if (tempCombo.size() < bestCombo.size()) better = true;
                    else if (tempCombo.size() == bestCombo.size() && tempCombo < bestCombo)
                        better = true;
                }

                if (better) {
                    isSelected = candidate;
                    currProfit = tempProfit;
                    currWeight = tempWeight;
                    currCount = tempCount;
                    bestCombo = tempCombo;
                    improved = true;
                    break;
                }
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Hybrid Knapsack Solver Runtime: " << elapsed.count() << " seconds\n";
    std::cout << "Total Profit: " << currProfit << "\n";
    std::cout << "Selected Pallets (IDs): ";
    for (int id : bestCombo) std::cout << id << " ";
    std::cout << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return currProfit;
}




