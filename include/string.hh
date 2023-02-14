#ifndef STRING_HH
#define STRING_HH

#include <string>
#include <exception>
#include <stdexcept>

#include "variable.hh"
#include "number.hh"

namespace fell {
    namespace types {
        struct string : public variable {
            string(std::string = "");

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
