#include "dv.hpp"
#include <iostream>

int main() {
    dv::DV d(10.0, "m");
    dv::DV t(2.0, "s");
    dv::DV v = d / t; // 5 m/s
    double mph = v.value_in("mi/hr");
    std::cout << "speed = " << mph << " mi/hr\n";
    return 0;
}
