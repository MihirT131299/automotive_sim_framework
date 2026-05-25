#include "ev_model.h"
#include <iostream>

EV_Model::EV_Model()
    : mass(0.0), max_force(0.0), drag_coefficient(0.0), velocity(0.0), connector(nullptr) {}

void EV_Model::load_params(const Config& config) {
    mass = config.get("vehicle_params", "mass");
    max_force = config.get("vehicle_params", "max_force");
    drag_coefficient = config.get("vehicle_params", "drag_coefficient");
}

void EV_Model::setConnector(BusConnector* c) {
    connector = c;
}

void EV_Model::calculate_velocity(double dt) {
    if (connector == nullptr) {
        std::cerr << "EV_Model: no connector set\n";
        return;
    }
    double throttle = connector->read("throttle");
    double propulsion_force = throttle * max_force;
    double drag_force = drag_coefficient * velocity * velocity;
    double net_force = propulsion_force - drag_force;
    double ax = net_force / mass;
    velocity = velocity + ax * dt;
    connector->write("velocity", velocity);
}

double EV_Model::get_velocity() const {
    return velocity;
}