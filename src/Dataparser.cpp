
#include "Dataparser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Truck DataParser::parseTruck(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open truck file: " + filename);
    }

    std::string line;
    bool headerSkipped = false;

    while (std::getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        auto tokens = splitCSVLine(line);
        if (tokens.size() < 2) continue;

        return Truck(
            std::stoi(trim(tokens[0])),  // Capacity
            std::stoi(trim(tokens[1]))   // Max pallets
        );
    }

    throw std::runtime_error("No valid truck data found in file: " + filename);
}

std::vector<Pallet> DataParser::parsePallets(const std::string& filename) {
    std::vector<Pallet> pallets;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open pallets file: " + filename);
    }

    std::string line;
    bool headerSkipped = false;

    while (std::getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        auto tokens = splitCSVLine(line);
        if (tokens.size() < 3) continue;

        pallets.emplace_back(
            std::stoi(trim(tokens[0])),  // ID
            std::stoi(trim(tokens[1])),  // Weight
            std::stoi(trim(tokens[2]))   // Profit
        );
    }

    return pallets;
}

std::vector<std::string> DataParser::splitCSVLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);

    while (std::getline(tokenStream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

std::string DataParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}