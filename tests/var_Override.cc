#include "variable.hh"
#include "util.hh"

#include <iostream>

using var = fell::types::variable::var;

int main() {
    var a = fell::util::make_var<fell::types::string>("Hello World!");
    var b = fell::util::make_var<fell::types::number>(42);

    std::cout << "b of type Number: " << fell::util::get_value<fell::types::number::num>(b) << '\n';
    std::cout << "a of type String: " << fell::util::get_value<fell::types::string::str>(a) << '\n';

    fell::util::override(a, b);

    std::cout << "a of type String was overriden to Number: "<< fell::util::get_value<fell::types::number::num>(a) << '\n';
}
