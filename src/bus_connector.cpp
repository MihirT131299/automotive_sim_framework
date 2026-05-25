#include "bus_connector.h"

BusConnector::BusConnector(SignalBus* bus, const std::string& identity)
    : bus(bus), model_identity(identity) {}

void BusConnector::registerWrite(const std::string& internal_name, const std::string& bus_name) {
    write_map[internal_name] = bus_name;
    bus->registerWriter(bus_name, model_identity);
}

void BusConnector::registerRead(const std::string& internal_name, const std::string& bus_name) {
    read_map[internal_name] = bus_name;
}

void BusConnector::write(const std::string& internal_name, double value) {
    if (!write_map.count(internal_name)) {
        std::cerr << "Write failed: internal signal '" << internal_name << "' not in write map\n";
        return;
    }
    bus->write(write_map[internal_name], value, model_identity);
}

double BusConnector::read(const std::string& internal_name) const {
    if (!read_map.count(internal_name)) {
        std::cerr << "Read failed: internal signal '" << internal_name << "' not in read map\n";
        return 0.0;
    }
    return bus->read(read_map.at(internal_name));
}