#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <vector>

class Config {
private:
    std::map<std::string, std::map<std::string, double>> config_data;
    std::map<std::string, std::map<std::string, std::string>> string_data;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> array_data;

public:
    void load(const std::string& configFileName);
    double get(const std::string& section, const std::string& key) const;
    std::string getString(const std::string& section, const std::string& key) const;
    std::vector<std::string> getArray(const std::string& section, const std::string& key) const;
};

#endif