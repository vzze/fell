#ifndef TABLE_HH
#define TABLE_HH

#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <string>

#include "variable.hh"
#include "number.hh"

namespace std {
    template<>
    struct hash<const fell::types::variable::var &> {
        std::size_t operator () (const fell::types::variable::var & var) {
            return std::hash<std::string>{}(
                std::any_cast<std::string>(var->value)
            );
        }
    };
}

namespace fell {
    namespace types {
        struct table : public variable {
            table(std::unordered_map<variable::var, variable::var>);

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
        };
    }
}

#endif
