#include "table.hh"
#include "string.hh"
#include "number.hh"
#include "override.hh"

#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>

using var = fell::types::variable*;

void test_msg(std::string msg, var a) {
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

std::vector<std::function<void(var&)>> tests = {
    [](var & table) {
        try {
            *table + table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator +  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            *table - table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            *table * table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            *table / table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table > table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table >= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >= : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table < table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <  : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table <= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <= : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table == table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator == : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
        try {
            [[maybe_unused]] auto s = *table != table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator != : ") + e.what(), nullptr);
        }
    },
    [](var & table) {
            test_msg("Operator [] :", nullptr);

            var key1 = new fell::types::string("Apple");
            var key2 = new fell::types::string("Apricot");

            (*table)[key1] = new fell::types::string("Cherry");
            (*table)[key2] = new fell::types::string("Orange");

            test_msg("\tKey 1 : ", key1);
            test_msg("\tKey 2 : ", key2);

            test_msg("\tTable values: ", nullptr);
            std::cout << "\t\t{ " << std::any_cast<fell::types::string::str>(key1->value) << ", " << std::any_cast<fell::types::string::str>((*table)[key1]->value) << " }\n";
            std::cout << "\t\t{ " << std::any_cast<fell::types::string::str>(key2->value) << ", " << std::any_cast<fell::types::string::str>((*table)[key2]->value) << " }\n";

            fell::util::override((*table)[key1], (*table)[key2]);

            std::cout << "\n\tOverriding value at Key 1 with value at Key 2:\n\n";

            test_msg("\tTable values: ", nullptr);
            std::cout << "\t\t{ " << std::any_cast<fell::types::string::str>(key1->value) << ", " << std::any_cast<fell::types::string::str>((*table)[key1]->value) << " }\n";
            std::cout << "\t\t{ " << std::any_cast<fell::types::string::str>(key2->value) << ", " << std::any_cast<fell::types::string::str>((*table)[key2]->value) << " }\n";

            delete key1;
            delete key2;
    },
};

int main() {
    fell::types::variable * table = new fell::types::table();

    for(auto test : tests)
        test(table);

    delete table;
}
