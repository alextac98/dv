#include "dv.hpp"
#include <iostream>

int main() {
    dv::DV mass(10.0, "kg");
    dv::DV accel(9.81, "m/s^2");
    dv::DV force = mass * accel; // N
    std::cout << "force = " << force.value_in("lbf") << " lbf\n";
    return 0;
}
