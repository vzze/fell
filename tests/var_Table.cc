#include "table.hh"
#include "string.hh"
#include "number.hh"

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
            std::cout << std::any_cast<double>(a->value) << '\n';
        }
    }
}

std::vector<std::function<void(var*)>> tests = {
    [](var * table) {
        try {
            *table + table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator +  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            *table - table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator -  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            *table * table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator *  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            *table / table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator /  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table > table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table >= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator >= : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table < table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <  : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table <= table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator <= : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table == table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator == : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
        try {
            [[maybe_unused]] auto s = *table != table;
        } catch(std::exception & e) {
            test_msg(std::string("Operator != : ") + e.what(), nullptr);
        }
    },
    [](var * table) {
            test_msg("Operator [] :", nullptr);
            var * key1 = new fell::types::string("Apple");
            var * key2 = new fell::types::string("Apricot");

            test_msg("\tKey 1 : ", key1);
            test_msg("\tKey 2 : ", key2);

            (*table)[key1] = new fell::types::number {42};
            (*table)[key2] = new fell::types::string {"Cherry"};

            delete key1; // this tests for dangling lifetime
            delete key2;

            key1 = new fell::types::string("Apple");
            key2 = new fell::types::string("Apricot");

            test_msg("\tTable values: ", nullptr);
            std::cout << "\t\t{ " << std::any_cast<std::string>(key1->value) << ", " << std::any_cast<double>((*table)[key1]->value) << " }\n";
            std::cout << "\t\t{ " << std::any_cast<std::string>(key2->value) << ", " << std::any_cast<std::string>((*table)[key2]->value) << " }\n";

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
