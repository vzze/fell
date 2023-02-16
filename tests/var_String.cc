#include "string.hh"

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
            std::cout << std::any_cast<fell::types::string::str>(a->value) << '\n';
        } catch(...) {
            std::cout << std::any_cast<fell::types::number::num>(a->value) << '\n';
        }
    }
}

std::vector<std::function<void(var*, var*)>> tests = {
    [](var * a, var * b) {
        var * c = *a + b;
        test_msg("Operator +  : ", c);
        delete c;
    },
    [](var * a, var * b) {
        try {
            var * c = *a - b;
            test_msg("Operator -  : ", c);
            delete c;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what(), nullptr);
        }
    },
    [](var * a, var * b) {
        try {
            var * c = *a * b;
            test_msg("Operator *  : ", c);
            delete c;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what(), nullptr);
        }
    },
    [](var * a, var * b) {
        try {
            var * c = *a / b;
            test_msg("Operator /  : ", c);
            delete c;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what(), nullptr);
        }
    },
    [](var * a, var * b) {
        var * c = *a > b;
        test_msg("Operator >  : ", c);
        delete c;
    },
    [](var * a, var * b) {
        var * c = *a >= b;
        test_msg("Operator >= : ", c);
        delete c;
    },
    [](var * a, var * b) {
        var * c = *a < b;
        test_msg("Operator <  : ", c);
        delete c;
    },
    [](var * a, var * b) {
        var * c = *a <= b;
        test_msg("Operator <= : ", c);
        delete c;
    },
    [](var * a, var * b) {
        var * c = *a == b;
        test_msg("Operator == : ", c);
        delete c;
    },
    [](var * a, var * b) {
        var * c = *a != b;
        test_msg("Operator != : ", c);
        delete c;
    },
    [](var * a, var * b) {
        try {
            var * c = (*a)[b];
            test_msg("Operator [] : ", c);
            delete c;
        } catch(std::exception & e) {
            test_msg(std::string("Operator [] : ") + e.what(), nullptr);
        }
    },
};

int main() {
    fell::types::variable * a = new fell::types::string("Hello ");
    fell::types::variable * b = new fell::types::string("World!");

    std::cout << "Variable a: " << std::any_cast<fell::types::string::str>(a->value) << '\n';
    std::cout << "Variable b: " << std::any_cast<fell::types::string::str>(b->value) << "\n\n";

    for(auto test : tests)
        test(a, b);

    delete a;
    delete b;
}
