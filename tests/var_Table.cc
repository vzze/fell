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
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table + table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator +  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table - table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table * table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table / table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table % table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator %  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table > table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table >= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >= : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table < table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <  : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table <= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <= : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table == table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator == : ") + e.what());
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table != table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator != : ") + e.what());
        }
    },
    [](var & table) {
        test_msg("Operator [var&]  :");
        test_msg("Operator [var&&] :");

        var key1 = fell::util::make_var<fell::types::string>("Apple");
        var key2 = fell::util::make_var<fell::types::string>("Apricot");

        (*table)[key1] = fell::util::make_var<fell::types::number>(42);
        (*table)[fell::util::make_var<fell::types::string>("Apricot")] = fell::util::make_var<fell::types::string>("Orange");

        test_msg("\tKey 1 : ", key1);
        test_msg("\tKey 2 : ", key2);

        test_msg("\tTable values: ");

        std::cout << "\t\t{ " << fell::util::get_value<fell::types::string::str>(key1) << ", "
            << fell::util::get_value<fell::types::number::num>((*table)[key1]) << " }\n";

        std::cout << "\t\t{ " << fell::util::get_value<fell::types::string::str>(key2) << ", "
            << fell::util::get_value<fell::types::string::str>((*table)[key2]) << " }\n";

        fell::util::copy((*table)[key1], (*table)[key2]);

        std::cout << "\n\tOverriding value at Key 1 with value at Key 2:\n\n";

        test_msg("\tTable values: ");

        std::cout << "\t\t{ " << fell::util::get_value<fell::types::string::str>(key1) << ", "
            << fell::util::get_value<fell::types::string::str>((*table)[key1]) << " }\n";

        std::cout << "\t\t{ " << fell::util::get_value<fell::types::string::str>(key2) << ", "
            << fell::util::get_value<fell::types::string::str>((*table)[key2]) << " }\n";
    },
    [](var & table) {
        test_msg("Operator [std::string]: Key: \"Apricot\", Value: ", (*table)["Apricot"]);
    }
};

int main() {
    var table = fell::util::make_var<fell::types::table>();

    for(auto test : tests)
        test(table);

    test_msg("\nLang dump_table func:\n");

    fell::lang::dump_table(table);
}
