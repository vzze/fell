#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <any>
#include <cstddef>
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

            virtual ~variable();
        };

        void override(variable::var &, const variable::var);
    }
}

#endif
