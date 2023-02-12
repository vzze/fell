#ifndef TABLE_HH
#define TABLE_HH

#include <unordered_map>

#include "variable.hh"

namespace fell {
    namespace types {
        struct table : public variable {
            //                       v std::hash custom impl
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

            var operator [] (const var &) override;
        };
    }
}

#endif
