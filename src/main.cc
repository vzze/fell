#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <any>

#include "variable/string.hh"
#include "variable/number.hh"

void test_string() {
    fell::types::variable * a = new fell::types::string("Hello ");
    fell::types::variable * b = new fell::types::string("World!");

    fell::types::variable * c = *a + b;

    std::cout << std::any_cast<std::string>(c->value) << '\n';

    delete a;
    delete b;
    delete c;
}

void test_number() {
    fell::types::variable * a = new fell::types::number(21);
    fell::types::variable * b = new fell::types::number(21);

    fell::types::variable * c = *a + b;

    std::cout << std::any_cast<long double>(c->value);

    delete a;
    delete b;
    delete c;
}

void test_override() {
    fell::types::variable * a = new fell::types::string("Hello World!");
    fell::types::variable * b = new fell::types::number(42);

    fell::types::override(a, b);
    std::cout << std::any_cast<long double>(a->value) << '\n';

    delete a;
    delete b;
}

int main() {
    test_override();
}
