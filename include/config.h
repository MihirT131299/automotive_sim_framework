#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

class Config{
private:
    std::map<std::string, std::map<std::string, double>> config_data;


public:

    void load(std::string configFileName);
    double get(std::string section, std::string key);
};

#endif