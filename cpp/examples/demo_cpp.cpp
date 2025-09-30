#include "dv.hpp"
#include <iostream>

int main() {
    using namespace dvpp;
    Var d(10.0, "m");
    Var t(2.0, "s");
    Var v = d / t; // 5 m/s
    double mph = v.value_in("mi/hr");
    std::cout << "speed = " << mph << " mi/hr\n";
    return 0;
}
