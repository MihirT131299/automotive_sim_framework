#ifndef EV_MODEL_H
#define EV_MODEL_H

#include "config.h"
#include "bus_connector.h"

class EV_Model {
private:
    double mass;
    double max_force;
    double drag_coefficient;
    double velocity;
    BusConnector* connector;

public:
    EV_Model();
    void load_params(const Config& config);
    void setConnector(BusConnector* connector);
    void calculate_velocity(double dt);
    double get_velocity() const;
};

#endif