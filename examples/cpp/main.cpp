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
    
    // Free-standing inverse trig functions
    dv::DV angle_from_asin = dv::asin(0.5);
    dv::DV angle_from_acos = dv::acos(0.5);
    dv::DV angle_from_atan = dv::atan(1.0);
    std::cout << "asin(0.5) = " << angle_from_asin.value_in("rad") << " rad\n";
    std::cout << "acos(0.5) = " << angle_from_acos.value_in("rad") << " rad\n";
    std::cout << "atan(1.0) = " << angle_from_atan.value_in("deg") << " deg (should be 45)\n";
    
    return 0;
}
