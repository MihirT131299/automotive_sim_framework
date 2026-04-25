#include <iostream>
#include "logger.h"
#include "config.h"
#include "ev_model.h"

int main(int argc, char* argv[]){
    std::string configFile = "";
    if (argc > 2){
        if (std::string(argv[1]) == "--config") {
            configFile = argv[2];
        } else {
            std::cerr << "Unknown command: use --config to specify config file path";
            return 1;
        }
        
    } else if (argc == 1) {
        std::cerr << "No config file selected. Please specify path to config file to be used for simulation. \n";
        return 1;
    } else {
        std::cerr << "Incorrect call made: please use <executable> --config <path to config file>";
        return 1;
    }
    
    std::cout << "Simulation starting..." << "\n";
    Config config;
    config.load(configFile);
    double t_start = config.get("simulation_loop_settings","t_start");
    double t_end = config.get("simulation_loop_settings","t_end");
    double t_current = 0.0;
    double dt = config.get("simulation_loop_settings","dt");
    
    Logger logFile;
    
    // Open the log file and write the header
    logFile.open("simulation_output.csv");
    logFile.writeHeader();

    // Creating EV model object
    EV_Model long_model;
    long_model.load_params(config);

    // Define constant throttle input
    double throttle = 0.5;

    // Simulation loop
    for(int step = 0; step < (int)((t_end - t_start) / dt); step++) {
        
        // Increment the current time by the time step
        t_current = dt * step;
        long_model.calculate_velocity(throttle,dt);
        double velo_current = long_model.get_velocity();
        
        // Write the current time to the log file
        logFile.writeRow(t_current, velo_current);

    }
    
    logFile.close();

    return 0;


}