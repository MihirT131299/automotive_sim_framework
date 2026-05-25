#include <iostream>
#include "config.h"
#include "engine.h"

int main(int argc, char* argv[]) {
    std::string configFile = "";

    if (argc > 2) {
        if (std::string(argv[1]) == "--config") {
            configFile = argv[2];
        } else {
            std::cerr << "Unknown command: use --config to specify config file path\n";
            return 1;
        }
    } else if (argc == 1) {
        std::cerr << "No config file selected. Please specify path to config file.\n";
        return 1;
    } else {
        std::cerr << "Incorrect call: use <executable> --config <path to config file>\n";
        return 1;
    }

    std::cout << "Simulation starting...\n";

    Config config;
    config.load(configFile);

    Engine engine(config);
    engine.run();

    return 0;
}