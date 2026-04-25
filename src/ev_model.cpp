#include "ev_model.h"
#include <iostream>

EV_Model::EV_Model() {
    velocity = 0.0;
}

void EV_Model::load_params(Config& config) {
    mass = config.get("vehicle_params","mass");
    max_force = config.get("vehicle_params","max_force");
    drag_coefficient = config.get("vehicle_params","drag_coefficient");
}

void EV_Model::calculate_velocity(double throttle, double dt) {
    double propulsion_force = throttle * max_force;
    double drag_force = drag_coefficient * velocity * velocity;
    double net_force = propulsion_force - drag_force;
    double ax = net_force/mass;
    velocity = velocity + ax*dt;
}

double EV_Model::get_velocity() {
    return velocity;
}