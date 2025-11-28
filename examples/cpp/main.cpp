#include "dv.hpp"
#include <iostream>
#include <cmath>

int main() {
    dv::DV mass(10.0, "kg");
    dv::DV accel(9.81, "m/s^2");
    dv::DV force = mass * accel; // N
    std::cout << "force = " << force.value_in("lbf") << " lbf\n";
    
    // Working with angles
    dv::DV angle_rad(M_PI / 4.0, "rad");
    dv::DV angle_deg(45.0, "deg");
    std::cout << "45 degrees = " << angle_deg.value_in("rad") << " radians\n";
    std::cout << "π/4 radians = " << angle_rad.value_in("deg") << " degrees\n";
    
    return 0;
}
