#ifndef TABLE_HH
#define TABLE_HH

#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <string>

#include "variable.hh"
#include "number.hh"
#include "string.hh"

namespace fell {
    namespace types {
        struct table : public variable {
            using tbl = std::unordered_map<std::string, variable::var>;
            table(tbl = {});

            var operator + (const var &) override;
            var operator - (const var &) override;
            var operator * (const var &) override;
            var operator / (const var &) override;

            var operator >  (const var &) override;
            var operator >= (const var &) override;
            var operator <  (const var &) override;
            var operator <= (const var &) override;
            var operator == (const var &) override;
            var operator != (const var &) override;

            var & operator [] (const var &) override;
            var & operator [] (const string::str) override;

            ~table();
        };
    }
}

#endif
