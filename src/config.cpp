#include "config.h"
#include <toml++/toml.hpp>
#include <iostream>

static void routeValue(
    const std::string& section,
    const std::string& key,
    const toml::node& value,
    std::map<std::string, std::map<std::string, double>>& config_data,
    std::map<std::string, std::map<std::string, std::string>>& string_data,
    std::map<std::string, std::map<std::string, std::vector<std::string>>>& array_data)
{
    if (value.is_number()) {
        config_data[section][key] = value.value_or(0.0);
    } else if (value.is_string()) {
        string_data[section][key] = value.value_or(std::string(""));
    } else if (value.is_array()) {
        std::vector<std::string> arr;
        for (auto& element : *value.as_array()) {
            arr.push_back(element.value_or(std::string("")));
        }
        array_data[section][key] = arr;
    } else {
        std::cerr << "Unrecognized value type for key '" << key << "' in section '" << section << "'\n";
    }
}

void Config::load(const std::string& configFileName) {
    auto config = toml::parse_file(configFileName);

    // Parse all sections and route each value to the correct storage map
    for (auto& [section_name, section_data] : config) {
        for (auto& [key, value] : *section_data.as_table()) {
            routeValue(std::string(section_name), std::string(key), value, config_data, string_data, array_data);
        }
    }
}

double Config::get(const std::string& section, const std::string& key) const {
    if (!config_data.count(section) || !config_data.at(section).count(key)) {
        std::cerr << "Double value not found: [" << section << "] " << key << "\n";
        return 0.0;
    }
    return config_data.at(section).at(key);
}

std::string Config::getString(const std::string& section, const std::string& key) const {
    if (!string_data.count(section) || !string_data.at(section).count(key)) {
        std::cerr << "String value not found: [" << section << "] " << key << "\n";
        return "";
    }
    return string_data.at(section).at(key);
}

std::vector<std::string> Config::getArray(const std::string& section, const std::string& key) const {
    if (!array_data.count(section) || !array_data.at(section).count(key)) {
        std::cerr << "Array value not found: [" << section << "] " << key << "\n";
        return {};
    }
    return array_data.at(section).at(key);
}