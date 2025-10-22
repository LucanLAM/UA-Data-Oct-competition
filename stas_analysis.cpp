#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <cmath>
#include <map>

namespace UAData {
    std::vector<std::string> names;
    std::vector<std::string> card_types;
    std::vector<float> tiers;
    std::vector<bool> results_iswin;
}

struct record {
    std::string playerName;
    std::string cardType;
    float tier;
    bool result; // 1 = win, 0 = loss
};

struct Round {
    record record1;
    record record2;
};

struct Stats {
    int total = 0; // how many rounds
    int wins = 0;  // how many wins
};

bool loadUADataFromCSV(const std::string& filename);

int main() {
    std::string filename = "UAData.csv";
    loadUADataFromCSV(filename);

    std::vector<Round> rounds;
    std::vector<Round> rounds_interested;
    std::map<float, Stats> tier_stats;

    for (size_t i = 0; i + 1 < UAData::names.size(); i += 2) {
        record r1 = {
            UAData::names[i],
            UAData::card_types[i],
            UAData::tiers[i],
            UAData::results_iswin[i]
        };
        record r2 = {
            UAData::names[i + 1],
            UAData::card_types[i + 1],
            UAData::tiers[i + 1],
            UAData::results_iswin[i + 1]
        };

        rounds.push_back({r1, r2});
    }
    for (size_t i = 0; i < rounds.size(); ++i) {
        const auto& round = rounds[i];
        // std::cout << "Round " << i + 1 << ":\n";
        // std::cout << "  Player 1: " << round.record1.playerName
        //         << ", Card: " << round.record1.cardType
        //         << ", Tier: " << round.record1.tier
        //         << ", Result: " << (round.record1.result ? "Win" : "Loss") << "\n";
        // std::cout << "  Player 2: " << round.record2.playerName
        //         << ", Card: " << round.record2.cardType
        //         << ", Tier: " << round.record2.tier
        //         << ", Result: " << (round.record2.result ? "Win" : "Loss") << "\n\n";
        float tier_difference = std::abs(round.record1.tier - round.record2.tier);
        if (tier_difference > 0) {
            rounds_interested.push_back(round);
        }
        float key = std::round(tier_difference * 2) / 2.0f;
        tier_stats[key].total++;
        if (((round.record1.result == 0 && (round.record1.tier<round.record2.tier))||
        (round.record1.result == 1) && (round.record1.tier>round.record2.tier))) {
            tier_stats[key].wins++;
        }
        
    }

    for (int i = 0; i < rounds_interested.size(); i++) {
        const auto& round = rounds_interested[i];
        std::cout << "Round " << i + 1 << ":\n";
        std::cout << "  Player 1: " << round.record1.playerName
                << ", Card: " << round.record1.cardType
                << ", Tier: " << round.record1.tier
                << ", Result: " << (round.record1.result ? "Win" : "Loss") << "\n";
        std::cout << "  Player 2: " << round.record2.playerName
                << ", Card: " << round.record2.cardType
                << ", Tier: " << round.record2.tier
                << ", Result: " << (round.record2.result ? "Win" : "Loss") << "\n\n";
    }
    int number_of_round_interested = rounds_interested.size();
    for (auto& [tier_diff, stats] : tier_stats) {
        if (tier_diff == 0) continue;
        float win_rate = (stats.total > 0)
            ? static_cast<float>(stats.wins) / stats.total
            : 0.0f;
        std::cout << "Tier diff " << tier_diff << ": "
                << "Rounds = " << stats.total
                << ", Win rate = " << win_rate << "\n";
        std::cout << "Expected value: " << win_rate * 10 *(1 + tier_diff) - 10 * (1 - win_rate) << "\n";
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


