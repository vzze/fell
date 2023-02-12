#include "string.hh"

#include <iostream>

int main() {
    fell::types::variable * a = new fell::types::string("Hello ");
    fell::types::variable * b = new fell::types::string("World!");

    fell::types::variable * c = *a + b;

    std::cout << "string data type: " << std::any_cast<std::string>(c->value) << '\n';

    delete a;
    delete b;
    delete c;
}
