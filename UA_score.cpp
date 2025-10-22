#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <cmath>

namespace UAData {
    std::vector<std::string> names;
    std::vector<std::string> card_types;
    std::vector<float> tiers;
    std::vector<bool> results_iswin;
}
std::vector<std::string> players_name = {"TTW", "LYP", "YMH", "YWH", "Eric", "Nam", "Francis", "Ray"};

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

class Player {
public:
    std::string name = "nan";
    int gamesPlayed = 0;
    int wins = 0;
    int losses = 0;
    int score = 40;

    float getWinRate() const {
        return (wins + losses > 0)
            ? static_cast<float>(wins) / (wins + losses)
            : 0.0f;
    }

    std::vector<int> score_records;
};

bool loadUADataFromCSV(const std::string& filename);
void printUAData();
Player* findPlayer(std::vector<Player>& Players, const std::string& name);

int main() {
    std::string filename = "UAData.csv";
    loadUADataFromCSV(filename);

    // printUAData();

    // store data in to round
    std::vector<Round> rounds;

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

    // store player into class
    std::vector<Player> Players;
    for (int i = 0; i < players_name.size(); i++) {
        Player p;                 // create a new Player object
        p.name = players_name[i];  // assign the name
        Players.push_back(p);     // store it into the vector
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
        // std::cout << "tier_difference: " << tier_difference << "\n";
        // std::cout << "tier_difference: " << round.record1.tier - round.record2.tier << "\n";
        int player1_score = 0;
        int player2_score = 0;
        if (tier_difference > 0 && ((round.record1.result == 0 && (round.record1.tier<round.record2.tier))||
        (round.record1.result == 1) && (round.record1.tier>round.record2.tier))) {
            // std::cout << "Trigger complex score calculation" << "\n";
            player1_score = 10 + static_cast<int>(tier_difference * 10);
        } else {
            // std::cout << "Trigger simple score calculation" << "\n";
            player1_score = 10;
        }
        if (round.record1.result == 0) player1_score = -player1_score;
        player2_score = - player1_score;

        // Test case
        if (player2_score + player1_score != 0) {
            std::cout << "Score sum wrong";
            return -1;
        }
        if ((player1_score > 0 && round.record1.result == 0) || (player2_score > 0 && round.record2.result == 0)) {
            std::cout << "player score should be negative";
            return -1;
        }
        if ((player1_score < 0 && round.record1.result != 0) || (player2_score < 0 && round.record2.result != 0)) {
            std::cout << "player score should be positive";
            return -1;
        }
        // std::cout << "player1_score: " << player1_score << "\n";
        // std::cout << "player2_score: " << player2_score << "\n";

        // --- Store score into Player objects ---
        Player* p1 = findPlayer(Players, round.record1.playerName);
        Player* p2 = findPlayer(Players, round.record2.playerName);

        for (auto& p : Players) {
            // Add 0 for players not in the round
            if (p.name != round.record1.playerName && p.name != round.record2.playerName) {
                p.score_records.push_back(0);
                continue;
            }
            if (p1 && p.name == p1->name) {
                p.score_records.push_back(player1_score);
                p.score += player1_score;
                if (p.score < 0) {
                    p.score = 0;
                }
                p.gamesPlayed++;
                if (round.record1.result) p.wins++; else p.losses++;
            }
            if (p2 && p.name == p2->name) {
                p.score_records.push_back(player2_score);
                p.score += player2_score;
                if (p.score < 0) {
                    p.score = 0;
                }
                p.gamesPlayed++;
                if (round.record2.result) p.wins++; else p.losses++;
            }
        }

        // for (const auto& p : Players) {
        //     std::cout << p.name
        //             << " | Total Score: " << p.score
        //             << " | WinRate: " << p.winRate * 100 << "%"
        //             << " | Scores per round: ";
        //     // for (int s : p.score_records) std::cout << s << " ";
        //     std::cout << "\n";
        // }
        // std::cout << "Press Enter to Continue"; 
        // std::cin.ignore();
        // std::cout << "-----------------------------------------------\n";
        
    }

    for (const auto& p : Players) {
        std::cout << p.name
                << " | Total Score: " << p.score
                << " | WinRate: " << p.getWinRate() * 100 << "%";
                // << " | Scores per round: ";
        // for (int s : p.score_records) std::cout << s << " ";
        std::cout << "\n";
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

Player* findPlayer(std::vector<Player>& Players, const std::string& name) {
    for (auto& p : Players) {
        if (p.name == name)
            return &p;
    }
    return nullptr;
}
