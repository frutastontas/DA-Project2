#include "Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <chrono>

Menu::Menu() : dataLoaded(false), resultsAvailable(false) {}

void Menu::showMainMenu() {
    while(true) {
        displayHeader("MAIN MENU");

        std::cout << "1. Load Data Files\n"
                  << "2. Select Solver Algorithm\n"
                  << "3. View Results\n"
                  << "4. Exit\n\n"
                  << "Select option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1:
                loadData();
                break;
            case 2:
                solverSelectionMenu();
                break;
            case 3:
                displayResults();  // View results
                break;
            case 4:
                exitProgram();
                return;
            default:
                std::cout << "Invalid option!\n";
                pressToContinue();
                break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Optional: add slight delay
    }
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
              << "5. Hybrid\n"
              << "6. Return to Main Menu\n\n"
              << "Select solver type: ";

    int choice;
    std::cin >> choice;

    SolverResult result;
    result.solverName = "Unknown";

    auto start = std::chrono::high_resolution_clock::now();

    try {
        switch(choice) {
            case 1:
                result.solverName = "Approximation";
                result.result= KnapsackApproximation(truck, pallets);
                break;
            case 2:
                result.solverName = "Backtracking";
                result.result = solveCase1(this->truck, this->pallets);
                break;
            case 3:
                result.solverName = "Branch and Bound";
                result.result = solveCase2(this->truck, this->pallets);
                break;
            case 4:
                result.solverName = "Dynamic Programming";
                result.result = solveDP(this->pallets, this->truck);
                break;
            case 5:
                result.solverName = "Hybrid";
                result.result = solveHybridKnapsack(this->truck, this->pallets);
                break;
            case 6:
                return;
            default:
                throw std::invalid_argument("Invalid selection");
        }


        auto end = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        resultsMap[currentDatasetId].push_back(result);

        std::cout << "Solver selected successfully!\n";
    } catch(...) {
        std::cout << "Invalid solver selection!\n";
    }

    pressToContinue();
}

void Menu::displayResults() {
    std::cout << "Enter dataset identifier to view results: ";
    std::string datasetId;
    std::getline(std::cin, datasetId);

    if (resultsMap.find(datasetId) == resultsMap.end()) {
        std::cout << "No results available for this dataset.\n";
        pressToContinue();
        return;
    }

    std::cout << "\nResults for Dataset: " << datasetId << "\n";
    for (const auto& result : resultsMap[datasetId]) {
        std::cout << "Solver: " << result.solverName << "\n";
        std::cout << "Result: " << result.result << "\n";
        std::cout << "Execution Time: " << result.duration.count() << " seconds\n";
        std::cout << "----------------------------------------\n";
    }
    pressToContinue();
}

void Menu::displayHeader(const std::string& title) const {
    std::cout << "\n=== " << title << " ===\n\n";
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

    std::string truckFile = "../datasets/TruckAndPallets_" + currentDatasetId + ".csv";
    std::string palletFile = "../datasets/Pallets_" + currentDatasetId + ".csv";

    try {
        this->truck = DataParser::parseTruck(truckFile);
        this->pallets = DataParser::parsePallets(palletFile);
        dataLoaded = true;
        resultsAvailable = false;
        std::cout << "Successfully loaded dataset " << currentDatasetId << "!\n"
                  << "Truck file: " << truckFile << "\n"
                  << "Pallet file: " << palletFile << "\n";
        std::cout << truck.getMaxCapacity() << " " << truck.getPalletsCapacity() << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error loading dataset " << currentDatasetId << ": " << e.what() << "\n";
        dataLoaded = false;
    }
    pressToContinue();
}
