#include "Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>

Menu::Menu() : dataLoaded(false), resultsAvailable(false) {}

void Menu::showMainMenu() {
    while(true) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        displayHeader("MAIN MENU");

        std::cout << "1. Load Data Files\n"
                  << "2. Select Solver Algorithm\n"
                  << "3. Compare to optimal\n"
                  << "6. Exit\n\n"
                  << "Select option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1: loadData(); break;
            case 2: solverSelectionMenu(); break;
            case 6: exitProgram(); return;
            default:
                std::cout << "Invalid option!\n";
                pressToContinue();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Menu::displayHeader(const std::string& title) const {
    std::cout << "\n=== " << title << " ===\n\n";
}

void Menu::displaySeparator() const {
    std::cout << "----------------------------------------\n";
}

void Menu::pressToContinue() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void Menu::solverSelectionMenu() {
    if (!dataLoaded) {
        std::cout << "No data loaded!\n";
        pressToContinue();
        return;
    }
    displayHeader("SELECT SOLVER");
    std::cout << "1. Approximation\n"
              << "2. Backtracking \n"
              << "3. Branch and Bound\n"
              << "4. Dynamic Programming\n"
              << "5. Return to Main Menu\n\n"
              << "Select solver type: ";

    int choice;
    std::cin >> choice;

    try {
        switch(choice) {
            case 1:
                KnapsackApproximation(truck, pallets);
                break;
            case 2:
                solveCase1(this->truck, this->pallets);
                break;
            case 3:
                solveCase2(this->truck, this->pallets);
                break;
            case 4:
                solveDP( this->pallets, this->truck);
                break;
            case 5:
                return;

            default: throw std::invalid_argument("Invalid selection");
        }
        std::cout << "Solver selected successfully!\n";
    } catch(...) {
        std::cout << "Invalid solver selection!\n";
    }
}

void Menu::exitProgram() {
    displayHeader("EXITING PROGRAM");
    std::cout << "Thank you for using Knapsack Solver!\n";
    pressToContinue();
}

void Menu::fileLoadingMenu() {
    displayHeader("DATA LOADING");
    std::cout << "Enter dataset identifier (X): ";
    std::string x;
    std::getline(std::cin, x);
    currentDatasetId = x;
}

void Menu::loadData() {
    fileLoadingMenu();

    if(currentDatasetId.empty()) {
        std::cout << "No dataset identifier provided!\n";
        pressToContinue();
        return;
    }

    std::string truckFile = "../data/datasets-extra/TruckAndPallets_" + currentDatasetId + ".csv";
    std::string palletFile = "../data/datasets-extra/Pallets_" + currentDatasetId + ".csv";

    try {
        this->truck = DataParser::parseTruck(truckFile);
        this->pallets = DataParser::parsePallets(palletFile);
        dataLoaded = true;
        resultsAvailable = false;
        std::cout << "Successfully loaded dataset " << currentDatasetId << "!\n"
                  << "Truck file: " << truckFile << "\n"
                  << "Pallet file: " << palletFile << "\n";
        std::cout << truck.getMaxCapacity()<< " " << truck.getPalletsCapacity()<< std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error loading dataset " << currentDatasetId << ": " << e.what() << "\n";
        dataLoaded = false;
    }
    pressToContinue();
}
