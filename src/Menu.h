#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "Dataparser.h"
#include "Solver.h"

class Menu {
public:
    Menu();
    void showMainMenu();

    // Main menu options
    void loadData();
    void selectSolver();
    void exitProgram();

private:
    Truck truck = Truck(0,0);
    std::vector<Pallet> pallets;
    std::string currentDatasetId;
    bool dataLoaded;
    bool resultsAvailable;

    // Data structure to store solver results
    struct SolverResult {
        std::string solverName;
        int result;
        std::chrono::duration<double> duration;
    };

    // Map to store results by dataset identifier
    std::map<std::string, std::vector<SolverResult>> resultsMap;

    // UI helpers
    void displayHeader(const std::string& title) const;
    void displaySeparator() const;
    void pressToContinue() const;

    // Sub-menus
    void solverSelectionMenu();
    void fileLoadingMenu();

    // New helper to display stored results
    void displayResults();
};

#endif // MENU_H
