#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <vector>
#include <string>
#include "classes/Truck.h"
#include "classes/Pallet.h"

class DataParser {
public:
    // Parses truck CSV and returns a single Truck
    static Truck parseTruck(const std::string& filename);

    // Parses pallets CSV and returns vector of Pallet objects with IDs
    static std::vector<Pallet> parsePallets(const std::string& filename);

private:
    static std::vector<std::string> splitCSVLine(const std::string& line);
    static std::string trim(const std::string& str);
};

#endif // DATAPARSER_H
