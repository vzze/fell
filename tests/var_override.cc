#include "string.hh"
#include "number.hh"

#include <iostream>

int main() {
    fell::types::variable * a = new fell::types::string("Hello World!");
    fell::types::variable * b = new fell::types::number(42);

    fell::types::override(a, b);
    std::cout << "a of type string was overriden to number: "<< std::any_cast<long double>(a->value) << '\n';

    delete a;
    delete b;
}
