#include "string.hh"
#include "number.hh"

#include <iostream>

int main() {
    fell::types::variable * a = new fell::types::string("Hello World!");
    fell::types::variable * b = new fell::types::number(42);

    std::cout << "b of type Number: " << std::any_cast<double>(b->value) << '\n';
    std::cout << "a of type String: " << std::any_cast<std::string>(a->value) << '\n';

    fell::types::override(a, b);

    std::cout << "a of type String was overriden to Number: "<< std::any_cast<double>(a->value) << '\n';

    delete a;
    delete b;
}
