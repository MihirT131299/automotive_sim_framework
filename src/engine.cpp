#include "engine.h"
#include <toml++/toml.hpp>
#include <iostream>

Engine::Engine(Config& config) : config(config) {}

Engine::~Engine() {
    for (auto& [name, connector] : connectors) {
        delete connector;
    }
}

void Engine::loadMappings(const std::vector<std::string>& model_list) {
    for (const auto& model_name : model_list) {
        std::string mapping_path = "interfaces/" + model_name + ".mapping.toml";
        auto mapping = toml::parse_file(mapping_path);

        BusConnector* connector = new BusConnector(&bus, model_name);

        // Register write signals and initialize on bus
        if (auto writes = mapping["writes"].as_table()) {
            for (auto& [internal_name, entry] : *writes) {
                std::string bus_name = "";
                double default_val = 0.0;
                if (auto* tbl = entry.as_table()) {
                    bus_name = tbl->get("bus_name")->value_or(std::string(""));
                    default_val = tbl->get("default")->value_or(0.0);
                }
                bus.initializeSignal(bus_name, default_val, model_name);
                connector->registerWrite(std::string(internal_name), bus_name);
            }
        }

        // Register read signals — only initialize on bus if no writer claimed it
        if (auto reads = mapping["reads"].as_table()) {
            for (auto& [internal_name, entry] : *reads) {
                std::string bus_name = "";
                double default_val = 0.0;
                if (auto* tbl = entry.as_table()) {
                    bus_name = tbl->get("bus_name")->value_or(std::string(""));
                    default_val = tbl->get("default")->value_or(0.0);
                }
                if (!bus.has(bus_name)) {
                    bus.initializeSignal(bus_name, default_val, "default");
                }
                connector->registerRead(std::string(internal_name), bus_name);
            }
        }

        connectors[model_name] = connector;
    }
}

void Engine::run() {
    // Load model list from already-parsed config
    std::vector<std::string> model_list = config.getArray("engine", "models");

    // Build signal bus from mapping files
    loadMappings(model_list);

    // Wire connectors to models
    // Execution order hardcoded here — upgrade to config-driven in a future chapter
    ev_model.load_params(config);
    ev_model.setConnector(connectors["ev_model"]);

    // Simulation parameters from config
    double t_start = config.get("simulation_loop_settings", "t_start");
    double t_end = config.get("simulation_loop_settings", "t_end");
    double dt = config.get("simulation_loop_settings", "dt");

    // Logger setup
    logger.open("simulation_output.csv");
    logger.writeHeader();

    double t_current = 0.0;

    // Simulation loop — hardcoded execution order
    for (int step = 0; step < (int)((t_end - t_start) / dt); step++) {
        t_current = dt * step;

        // Step 1: EV model reads throttle from bus, computes, writes velocity to bus
        ev_model.calculate_velocity(dt);

        // Step 2: Log outputs
        logger.writeRow(t_current, bus.read("ev.velocity"));
    }

    logger.close();
    std::cout << "Simulation complete.\n";
}