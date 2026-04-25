#ifndef EV_MODEL_H
#define EV_MODEL_H

#include <string>
#include "config.h"

class EV_Model{
private:
    double mass;
    double max_force;
    double drag_coefficient;
    double velocity;

public:

    void load_params(Config& config);
    void calculate_velocity(double throttle, double dt);
    double get_velocity();
    EV_Model();
};




#endif