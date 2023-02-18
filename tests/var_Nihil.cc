#include "variable.hh"
#include "util.hh"
#include "lang.hh"

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
        std::cout << std::any_cast<fell::types::string::str>(a->value) << '\n';
    } catch(...) {
        std::cout << std::any_cast<fell::types::number::num>(a->value) << '\n';
    }
}

void test_msg(std::string msg) {
    std::cout << msg << '\n';
}

std::vector<std::function<void(var&)>> tests = {
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil + nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator +  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil - nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil * nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil / nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil > nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil >= nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >= : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil < nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil <= nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <= : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil == nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator == : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto s = *nil != nil;
        } catch(std::exception & e) {
            test_msg(std::string("Operator != : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto & s = (*nil)[nil];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&]  : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto & s = (*nil)[fell::util::make_var<fell::types::string>("")];
        } catch(std::exception & e) {
            test_msg(std::string("Operator [var&&] : ") + e.what());
        }
    },
    [](var & nil) {
        try {
            [[maybe_unused]] auto & s = (*nil)["String"];
        } catch(std::exception & e) {

            test_msg(std::string("Operator [std::string]: ") + e.what());
        }
    }
};

int main() {
    var nil = fell::util::make_var<fell::types::nihil>();

    for(auto test : tests)
        test(nil);
}
