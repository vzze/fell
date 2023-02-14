#ifndef TABLE_HH
#define TABLE_HH

#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <string>

#include "variable.hh"
#include "number.hh"

namespace fell {
    namespace types {
        struct table : public variable {
            table(std::unordered_map<std::string, variable::var> = {});

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

            ~table();
        };
    }
}

#endif
