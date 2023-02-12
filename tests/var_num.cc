#include "number.hh"

#include <iostream>

int main() {
    fell::types::variable * a = new fell::types::number(21);
    fell::types::variable * b = new fell::types::number(21);

    fell::types::variable * c = *a + b;

    std::cout << "number data type: " << std::any_cast<long double>(c->value) << '\n';

    delete a;
    delete b;
    delete c;
}
