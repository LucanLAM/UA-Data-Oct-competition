#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace UAData {
    std::vector<std::string> names;
    std::vector<std::string> card_types;
    std::vector<float> tiers;
    std::vector<bool> results_iswin;
}

bool extractInfo(const std::string& line);
void trim(std::string& str);
void printUAData();

int main() {
    
    std::ifstream com_file("DataUA.txt");

    std::string line;
    
    bool isFitstLine = true;

    while (std::getline(com_file, line)) {
        if (line.empty()) continue;

        if (line.find("week") != std::string::npos) continue;

        if (line.find("Week") != std::string::npos) continue;

        if (!extractInfo(line)) return -1 ;
        
        if(UAData::results_iswin.size() > 1 && UAData::results_iswin.size() % 2 == 0) {
            if (!UAData::results_iswin[UAData::results_iswin.size() - 1] != UAData::results_iswin[UAData::results_iswin.size() - 2]) {
                std::cout << "UAData::results_iswin[-1]: " << UAData::results_iswin[UAData::results_iswin.size() - 1] << "\n";
                std::cout << "UAData::results_iswin[-2]: " << UAData::results_iswin[UAData::results_iswin.size() - 2] << "\n";
                return -1;
            }
        } 

    }

    // printUAData();
    std::cout << "Done" << "\n";

    std::ofstream out("UAData.csv");
    for (size_t i = 0; i < UAData::names.size(); ++i) {
        out << UAData::names[i] << ","
            << UAData::card_types[i] << ","
            << UAData::tiers[i] << ","
            << UAData::results_iswin[i] << "\n";
    }
    out.close();

    return 0;
}

bool extractInfo(const std::string& line) {
    // using namespace UAData;
    // std::cout << "Input line: " << line << "\n";
    std::string name, card_type, tier, result;
    float tier_num;
    bool is_win;
    size_t name_seperator1, 
            name_seperator2,
            name_seperator3,
            name_seperator4;


    // name_seperator1: in between name and card_type (whitespace or :)
    // name_seperator2: in between card_type and tier (t or T)
    // name_seperator3: in between tier and result (w, l)

    // find name
    name_seperator1 = line.find(" ");
    name_seperator2 = line.find(":");
    if (name_seperator1 == std::string::npos && name_seperator2 == std::string::npos) {
        std::cout << "Cannot find name in the following line:\n" << line << std::endl;
        return false;
    }
    if(name_seperator2 != std::string::npos && (name_seperator1 == std::string::npos || name_seperator2 < name_seperator1)) {
        name_seperator1 = name_seperator2;
    }
    // std::cout << "Debug for name_seperator1: " << name_seperator1 <<"\n";
    name = line.substr(0, name_seperator1);
    trim(name);
    // std::cout << "name: " << name << std::endl;

    // find card_type
    // find a 'T' or 't' followed by a number (0–9) or '.'
    name_seperator2 = std::string::npos;
    size_t search_start = name_seperator1 + 1;
    for (size_t i = search_start; i + 1 < line.size(); ++i) {
        if ((line[i] == 'T' || line[i] == 't') &&
            (std::isdigit(static_cast<unsigned char>(line[i + 1])) || line[i + 1] == '.')) {
            name_seperator2 = i;
            break;
        }
    }
    if (name_seperator2 == std::string::npos) {
        std::cout << "Cannot locate Tier in the following line:\n" << line << std::endl;
        return false;
    }
    
    // std::cout << "Debug for name_seperator2: " << name_seperator2 <<"\n";
    card_type = line.substr(name_seperator1 + 1, name_seperator2 - (name_seperator1 + 1));
    trim(card_type);
    // std::cout << "Card type: " << card_type << std::endl;

    // find tier
    const char win[] = {'w', '+'};
    const char loss[] = {'l', '-'};
    name_seperator3 = std::string::npos;
    
    for (int i = 0; i < sizeof(win); i++) {
        name_seperator4 = line.find(win[i]);
        if (name_seperator4 != std::string::npos && (name_seperator3 == std::string::npos || name_seperator4 < name_seperator3) ) {
            name_seperator3 = name_seperator4;
        }
    }

    if (name_seperator3 != std::string::npos) {
        result = "W";
    } else {
        for (int i = 0; i < sizeof(loss); i++) {
            name_seperator4 = line.find(loss[i]);
            if (name_seperator4 != std::string::npos && (name_seperator3 == std::string::npos || name_seperator4 < name_seperator3) ) {
                name_seperator3 = name_seperator4;
            }
        }
        if (name_seperator3 != std::string::npos) result = "L";
    }
    if (name_seperator3 == std::string::npos) {
        std::cout << "Cannot locate Result (W/L) in the following line:\n" << line << std::endl;
        return false;
    }
    // std::cout << "Debug for name_seperator3: " << name_seperator3 <<"\n";
    tier = line.substr(name_seperator2 + 1, name_seperator3 - (name_seperator2 + 1));
    trim(tier);
    try {
        tier_num = std::stof(tier);
        // UAData::tiers.push_back(value);
    } catch (const std::invalid_argument& e) {
        std::cerr << "In the line: " << line << "\n";
        std::cerr << "Invalid argument in std::stof: '" << tier << "'\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "In the line: " << line << "\n";
        std::cerr << "Out of range in std::stof: '" << tier << "'\n";
    }
    if (tier_num < 0.4 || tier_num > 5.1) {
        std::cout << "Tier not in range in the following line:\n" << line << std::endl;
        std::cout << "Tier: " << tier_num << std::endl;
        return false;
    }
    // tier_num = std::stof(tier);
    // std::cout << "tier: " << tier << std::endl;

    // find result
    // std::cout << "Result: " << result << std::endl;
    if (result == "W" || result == "L") {
        result == "W" ? is_win = true : is_win = false;
    } else {
        std::cout << "result not in range in the following line:\n" << line << std::endl;
        std::cout << "result: " << result << std::endl;
        return false;
    }

    //push all result into UAData
    // std::cout << "name: " << name << "\n" << "card_type: " << card_type << "\n" << "tier_num" << tier_num << "\n" << "is_win" << is_win << "\n\n";
    UAData::names.push_back(name);
    UAData::card_types.push_back(card_type);
    UAData::tiers.push_back(tier_num);
    UAData::results_iswin.push_back(is_win);

    return true;
}

void trim(std::string& str) {
    int length = str.length();
    int start = 0;
    while (std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    while (std::isspace(static_cast<unsigned char>(str[length - 1]))) {
        length--;
    }
    
    str = str.substr(start, length - start);
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
