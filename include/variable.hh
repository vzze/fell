#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <any>
#include <cstddef>
#include <string>
#include <cstring>

namespace fell {
    namespace types {
        struct variable {
            using var = variable*;

            std::any value;

            variable(std::any);

            virtual var operator + (const var &) = 0;
            virtual var operator - (const var &) = 0;
            virtual var operator * (const var &) = 0;
            virtual var operator / (const var &) = 0;

            virtual var operator >  (const var &) = 0;
            virtual var operator >= (const var &) = 0;
            virtual var operator <  (const var &) = 0;
            virtual var operator <= (const var &) = 0;
            virtual var operator == (const var &) = 0;
            virtual var operator != (const var &) = 0;

            virtual var & operator [] (const var &) = 0;
            virtual var & operator [] (const std::string) = 0;

            virtual ~variable();
        };
    }
}

#endif
