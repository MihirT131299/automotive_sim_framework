#ifndef BUS_CONNECTOR_H
#define BUS_CONNECTOR_H

#include <string>
#include <map>
#include "signal_bus.h"

class BusConnector {
private:
    SignalBus* bus;
    std::string model_identity;
    std::map<std::string, std::string> write_map; // internal → bus
    std::map<std::string, std::string> read_map;  // internal → bus

public:
    BusConnector(SignalBus* bus, const std::string& identity);

    void registerWrite(const std::string& internal_name, const std::string& bus_name);
    void registerRead(const std::string& internal_name, const std::string& bus_name);

    void write(const std::string& internal_name, double value);
    double read(const std::string& internal_name) const;
};

#endif