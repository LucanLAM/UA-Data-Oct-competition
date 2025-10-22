#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>

namespace UAData {
    std::vector<std::string> names;
    std::vector<std::string> card_types;
    std::vector<float> tiers;
    std::vector<bool> results_iswin;
}

bool loadUADataFromCSV(const std::string& filename);
void printUAData();

int main() {
    std::string filename = "UAData.csv";
    loadUADataFromCSV(filename);

    // printUAData();
    std::unordered_set<std::string> unique_names(UAData::names.begin(), UAData::names.end());
    std::unordered_set<std::string> unique_card_types(UAData::card_types.begin(), UAData::card_types.end());

    std::cout << "Unique names:\n";
    for (const auto& n : unique_names) {
        std::cout << n << "\n";
    }
    std::cout << "Unique card types:\n";
    for (const auto& n : unique_card_types) {
        std::cout << n << "\n";
    }

    return 0;
}

bool loadUADataFromCSV(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    // Clear any existing data
    UAData::names.clear();
    UAData::card_types.clear();
    UAData::tiers.clear();
    UAData::results_iswin.clear();

    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string name, card_type, tier_str, result_str;

        // Read 4 fields separated by commas
        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, card_type, ',')) continue;
        if (!std::getline(ss, tier_str, ',')) continue;
        if (!std::getline(ss, result_str, ',')) continue;

        // Convert tier to float
        float tier = 0;
        try {
            tier = std::stof(tier_str);
        } catch (...) {
            std::cerr << "Invalid tier: " << tier_str << " in line: " << line << "\n";
            continue;
        }

        // Convert result to bool
        bool is_win = (result_str == "1" || result_str == "true" || result_str == "W");

        // Store in UAData
        UAData::names.push_back(name);
        UAData::card_types.push_back(card_type);
        UAData::tiers.push_back(tier);
        UAData::results_iswin.push_back(is_win);
    }

    infile.close();
    return true;
}

void printUAData() {
    using namespace UAData;

    std::cout << "\n=== UAData Contents ===\n";
    // assuming all vectors are the same size
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "Record " << i + 1 << ":\n";
        std::cout << "  Name:      " << names[i] << "\n";
        std::cout << "  Card type: " << card_types[i] << "\n";
        std::cout << "  Tier:      " << tiers[i] << "\n";
        std::cout << "  Win?:      " << std::boolalpha << results_iswin[i] << "\n"; 
        std::cout << "-----------------------------\n";
    }

    std::cout << "Total records: " << names.size() << "\n";
}
