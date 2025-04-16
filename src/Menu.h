#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
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

    // UI helpers
    void displayHeader(const std::string& title) const;
    void displaySeparator() const;
    void pressToContinue() const;

    // Sub-menus
    void solverSelectionMenu();
    void fileLoadingMenu();

};

#endif // MENU_H