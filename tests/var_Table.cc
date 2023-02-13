#include "table.hh"

#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>

using var = fell::types::variable;

void test_msg(std::string msg, var * a) {
    std::cout << msg;
    if(a == nullptr)
        std::cout << '\n';
    else {
        try {
            std::cout << std::any_cast<std::string>(a->value) << '\n';
        } catch(...) {
            std::cout << std::any_cast<long double>(a->value) << '\n';
        }
    }
}

std::vector<std::function<void(var*, var*)>> tests = {

};

int main() {
    return 0;
}
