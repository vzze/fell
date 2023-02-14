#ifndef NUMBER_HH
#define NUMBER_HH

#include <exception>
#include <stdexcept>

#include "variable.hh"

namespace fell {
    namespace types {
        struct number : public variable {
            number(double = 0);

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
