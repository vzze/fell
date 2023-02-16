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
            using str = std::string;
            string(str = "");

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
            var & operator [] (const str) override;
        };
    }
}

#endif
