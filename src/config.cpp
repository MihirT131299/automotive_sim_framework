#include "config.h"
#include <toml++/toml.hpp>
#include <iostream>

void Config::load(std::string configFileName) {
    auto config = toml::parse_file(configFileName);

    // Parse all sections and key-value pairs from the TOML file into config_data
    for (auto& [section_name, section_data] : config) {
        for (auto& [key, value] : *section_data.as_table()) {
            double extracted = value.value_or(0.0);
            config_data[std::string(section_name)][std::string(key)] = extracted;


        }
    }
}

double Config::get(std::string section, std::string key) {
    if (!config_data.count(section) || !config_data[section].count(key)) {
        std::cerr << "Variable not found in config file. \n";
        return 0.0;
    } else {
        return config_data[section][key];
    }
}