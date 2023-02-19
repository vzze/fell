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
    try {
        std::cout << fell::util::get_value<fell::types::string::str>(a) << '\n';
    } catch(...) {
        std::cout << fell::util::get_value<fell::types::number::num>(a) << '\n';
    }
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
        try {
            [[maybe_unused]] var s = *a - b;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what());
        }
    },
    [](var & a, var & b) {
        try {
            [[maybe_unused]] var s = *a * b;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what());
        }
    },
    [](var & a, var & b) {
        try {
            [[maybe_unused]] var s = *a / b;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what());
        }
    },
    [](var & a, var & b) {
        try {
            [[maybe_unused]] var s = *a % b;
        } catch(std::exception & e) {
            test_msg(std::string("Operator %  : ") + e.what());
        }
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
            [[maybe_unused]] var & s = (*a)[b];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&]  : ") + e.what());
        }
    },
    [](var & a, [[maybe_unused]] var & b) {
        try {
            [[maybe_unused]] var & s = (*a)[fell::util::make_var<fell::types::number>(42)];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&&] : ") + e.what());
        }
    },
    [](var & a, [[maybe_unused]] var & b) {
        try {
            [[maybe_unused]] var & s = (*a)[std::string{}];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [std::string]: ") + e.what());
        }
    },
};

int main() {
    var a = fell::util::make_var<fell::types::string>("Hello ");
    var b = fell::util::make_var<fell::types::string>("World!");

    std::cout << "Variable a: " << fell::util::get_value<fell::types::string::str>(a) << '\n';
    std::cout << "Variable b: " << fell::util::get_value<fell::types::string::str>(b) << "\n\n";

    for(auto test : tests)
        test(a, b);
}
