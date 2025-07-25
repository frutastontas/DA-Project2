# DA-Project2
### Menu Interface
 - We made a menu that is able to load the files and data after being given a specific identifier for that data.
   
   ![image](https://github.com/user-attachments/assets/5c43c69e-a855-48b4-81dc-7b430d239303)

 - It also allows the user to select a specific solver, that they want to run for that dataset.

   ![image](https://github.com/user-attachments/assets/fc6f35e7-93b9-4d7e-a43c-f191b557013a)


 - Finally, we implemented a feature that allows the user to give an id and check the computations done so far.

   ![image](https://github.com/user-attachments/assets/2035ff20-241e-464d-b9aa-5e7ac73e9d56)


## Different Strategies
### Backtracking
     This is a brute force strategy where at each state of the decision tree we decide if we want to take the current pallet or not.
     This algorithm has time complexity of O(2^n) and space complexity of O(n), but it always returns the optimal solution to our problem.
     So we trade efficiency off for optimality. Since this is exponential it creates problems.
     Given a decently big dataset, the algorithm will not return since it´s doing a lot of computations, so it's infeasible.
     Still super useful for smaller datasets.
#### Branch and Bound
  - We are able to improve our backtracking by smartly pruning branches that are infeasible, meaning it won't give us a better result than our current one.
  - It still maintains the same complexity, but in practice is alot faster.
  - Still infeasible for big datasets.
##### Computing the upper bound
     To prune the branches we need a formula. 
     At a given state we will have the current sum and the remaining pallets. So by sorting the remaining pallets by their ratio we can make a theoretical upper bound 
     for that state. Basically do a fractional greedy Knapsack, where we can also take part of a pallet.
     In the case that the upper bound is less than our current best, we prune the branch.


### Dynamic Programming
     This strategy uses a bottom-up dynamic programming approach to solve the problem efficiently.
     We build a 2D table `dp[w][k]` that stores the maximum profit we can obtain with weight `w` and `k` pallets.
     For each pallet, we check whether including it gives a better profit.
     This results in a pseudo-polynomial algorithm, where it's polynomial on value of W, but exponential on size of W,
     because we need log(W) bits to represent it.
#### Data Structures
- A 2D vector `dp[w][k]` is used to store the maximum profit at each state.
- A 3D vector `chosen[w][k]` keeps track of which pallet IDs were used to reach each state.
- These allow efficient computation and reconstruction of the solution.
- Use the 2D table instead of 3D because of memory efficiency, despite being less intuitive.

#### Time and Space Complexity
- Time Complexity: O(W * K)
- Space Complexity: O(W * K)

#### Notes
- Returns the **optimal solution** under weight and pallet count constraints.
- Much faster than backtracking or branch and bound on large datasets.
- Memory usage increases with higher weight and pallet limits.

### Greedy Approximation
     This strategy runs two greedy heuristics: one that sorts the pallets by **profit**, and one by **profit-to-weight ratio**.
     It then selects the better of the two solutions. The idea is to quickly find a good (though not necessarily optimal) solution,
     which is especially useful for large datasets where exact methods are too slow.

     It does not guarantee optimality but performs well in practice and is extremely fast, making it useful for approximations
     or as part of a hybrid approach.

#### Data Structures
- Uses vectors to sort and store the pallets: `std::vector<Pallet>` and `std::vector<int>` for the selected IDs.
- Temporary copies are made to allow different sort orders.
- The sorting functions (`sortByProfit`, `sortByRatio`) enable flexible reuse.

#### Time and Space Complexity
- Time Complexity: O(n log n), due to sorting the pallet list twice.
- Space Complexity: O(n), for storing pallet combinations and copies of the list.

#### Notes
- **Does not guarantee the optimal solution.**
- Extremely **fast** even for large datasets.
- Useful when performance matters more than perfect accuracy.
- Picks between two heuristics: **max profit** and **best value per weight**.

### Hybrid Strategy
The **Hybrid Strategy** combines **greedy selection** and **local search improvement** to solve the 0/1 Knapsack problem efficiently. The approach works as follows:
- For small instances we use the Dynamic Programming;
- **Greedy Selection**: The pallets are initially selected based on their **profit-to-weight ratio**. This is a greedy approach where we select the most profitable pallets (per unit of weight) until we reach capacity constraints for the truck.
- **Local Search Improvement (Flipping)**: After the initial greedy selection, the algorithm performs a local search by **flipping** the inclusion of pallets one by one. It evaluates if toggling any selected pallet improves the overall solution in terms of profit. The search continues until no further improvement can be found.

#### Time and Space Complexity
- **Time Complexity**: O(n²) — The greedy selection takes O(n log n) for sorting, and the local search improvement involves iterating over pallets and checking solutions.
- **Space Complexity**: O(n) — Stores flags for selected pallets and a record of the best combination.

#### Notes
- **Greedy Selection**: Quickly generates an initial solution using a heuristic.
- **Local Search**: Tries to improve the greedy solution by flipping pallet inclusions, which can yield a better solution if improvements exist.
- **Efficient for medium-sized datasets**: The method strikes a balance between finding a good solution and not being as computationally expensive as other exact algorithms like Dynamic Programming.


