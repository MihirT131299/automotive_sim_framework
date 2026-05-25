#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <map>
#include "signal_bus.h"
#include "bus_connector.h"
#include "config.h"
#include "ev_model.h"
#include "logger.h"

class Engine {
private:
    SignalBus bus;
    Config& config;
    Logger logger;
    EV_Model ev_model;
    std::map<std::string, BusConnector*> connectors;

    void loadMappings(const std::vector<std::string>& model_list);
    void initializeDefaultSignals();

public:
    Engine(Config& config);
    ~Engine();
    void run();
};

#endif