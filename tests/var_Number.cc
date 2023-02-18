#include "variable.hh"
#include "util.hh"

#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>

using var = fell::types::variable::var;

void test_msg(std::string msg, var & a) {
    std::cout << msg;
    std::cout << fell::util::get_value<fell::types::number::num>(a) << '\n';
}

void test_msg(std::string msg) {
    std::cout << msg << '\n';
}

std::vector<std::function<void(var&, var&)>> tests = {
    [](var & a, var & b) {
        var c = *a + b;
        test_msg("Operator +  : ", c);
    },
    [](var & a, var & b) {
        var c = *a - b;
        test_msg("Operator -  : ", c);
    },
    [](var & a, var & b) {
        var  c = *a * b;
        test_msg("Operator *  : ", c);
    },
    [](var & a, var & b) {
        var c = *a / b;
        test_msg("Operator /  : ", c);
    },
    [](var & a, var & b) {
        var c = *a > b;
        test_msg("Operator >  : ", c);
    },
    [](var & a, var & b) {
        var c = *a >= b;
        test_msg("Operator >= : ", c);
    },
    [](var & a, var & b) {
        var c = *a < b;
        test_msg("Operator <  : ", c);
    },
    [](var & a, var & b) {
        var c = *a <= b;
        test_msg("Operator <= : ", c);
    },
    [](var & a, var & b) {
        var c = *a == b;
        test_msg("Operator == : ", c);
    },
    [](var & a, var & b) {
        var c = *a != b;
        test_msg("Operator != : ", c);
    },
    [](var & a, var & b) {
        try {
            [[maybe_unused]] var & c = (*a)[b];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&]  : ") + e.what());
        }
    },
    [](var & a, [[maybe_unused]] var & b) {
        try {
            [[maybe_unused]] var & c = (*a)[fell::util::make_var<fell::types::number>(21)];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&&] : ") + e.what());
        }
    },
    [](var & a, [[maybe_unused]] var & b) {
        try {
            [[maybe_unused]] var & c = (*a)[std::string{}];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [std::string]: ") + e.what());
        }
    },
};

int main() {
    var a = fell::util::make_var<fell::types::number>(20);
    fell::util::get_value<fell::types::number::num>(a) = 21;
    var b = fell::util::make_var<fell::types::number>(21);

    std::cout << "Variable a: " << fell::util::get_value<fell::types::number::num>(a) << '\n';
    std::cout << "Variable b: " << fell::util::get_value<fell::types::number::num>(b) << "\n\n";

    for(auto test : tests)
        test(a, b);
}
