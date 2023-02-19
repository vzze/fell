#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <string>
#include <memory>
#include <cmath>
#include <any>

namespace fell {
    namespace types {
        struct variable {
            // shorthand for std::unique_ptr<variable>
            using var = std::unique_ptr<variable>;

            std::any value;

            variable(std::any);

            [[nodiscard]] virtual var operator + (const var &) = 0;
            [[nodiscard]] virtual var operator - (const var &) = 0;
            [[nodiscard]] virtual var operator * (const var &) = 0;
            [[nodiscard]] virtual var operator / (const var &) = 0;
            [[nodiscard]] virtual var operator % (const var &) = 0;

            [[nodiscard]] virtual var operator >  (const var &) = 0;
            [[nodiscard]] virtual var operator >= (const var &) = 0;
            [[nodiscard]] virtual var operator <  (const var &) = 0;
            [[nodiscard]] virtual var operator <= (const var &) = 0;
            [[nodiscard]] virtual var operator == (const var &) = 0;
            [[nodiscard]] virtual var operator != (const var &) = 0;

            [[nodiscard]] virtual var & operator [] (const var &) = 0;
            [[nodiscard]] virtual var & operator [] (const var &&) = 0;
            [[nodiscard]] virtual var & operator [] (const std::string) = 0;

            virtual ~variable();
        };

        struct number : public variable {
            // underlying type stored in member value
            using num = double;
            number(num = 0);

            [[nodiscard]] var operator + (const var &) override;
            [[nodiscard]] var operator - (const var &) override;
            [[nodiscard]] var operator * (const var &) override;
            [[nodiscard]] var operator / (const var &) override;
            [[nodiscard]] var operator % (const var &) override;

            [[nodiscard]] var operator >  (const var &) override;
            [[nodiscard]] var operator >= (const var &) override;
            [[nodiscard]] var operator <  (const var &) override;
            [[nodiscard]] var operator <= (const var &) override;
            [[nodiscard]] var operator == (const var &) override;
            [[nodiscard]] var operator != (const var &) override;

            [[noreturn]] var & operator [] (const var &) override;
            [[noreturn]] var & operator [] (const var &&) override;
            [[noreturn]] var & operator [] (const std::string) override;
        };

        struct string : public variable {
            // underlying type stored in member value
            using str = std::string;
            string(str = "");

            [[nodiscard]] var operator + (const var &) override;
            [[noreturn]] var operator - (const var &) override;
            [[noreturn]] var operator * (const var &) override;
            [[noreturn]] var operator / (const var &) override;
            [[noreturn]] var operator % (const var &) override;

            [[nodiscard]] var operator >  (const var &) override;
            [[nodiscard]] var operator >= (const var &) override;
            [[nodiscard]] var operator <  (const var &) override;
            [[nodiscard]] var operator <= (const var &) override;
            [[nodiscard]] var operator == (const var &) override;
            [[nodiscard]] var operator != (const var &) override;

            [[noreturn]] var & operator [] (const var &) override;
            [[noreturn]] var & operator [] (const var &&) override;
            [[noreturn]] var & operator [] (const str) override;
        };

        struct table : public variable {
            // underlying type stored in member value
            using tbl = std::unordered_map<string::str, var>*;

            table(tbl = new std::unordered_map<string::str, var>());

            [[noreturn]] var operator + (const var &) override;
            [[noreturn]] var operator - (const var &) override;
            [[noreturn]] var operator * (const var &) override;
            [[noreturn]] var operator / (const var &) override;
            [[noreturn]] var operator % (const var &) override;

            [[noreturn]] var operator >  (const var &) override;
            [[noreturn]] var operator >= (const var &) override;
            [[noreturn]] var operator <  (const var &) override;
            [[noreturn]] var operator <= (const var &) override;
            [[noreturn]] var operator == (const var &) override;
            [[noreturn]] var operator != (const var &) override;

            [[nodiscard]] var & operator [] (const var &) override;
            [[nodiscard]] var & operator [] (const var &&) override;
            [[nodiscard]] var & operator [] (const string::str) override;

            ~table();
        };

        struct nihil : public variable {
            // underlying type stored in member value
            using nil = std::nullptr_t;
            nihil(nil = nullptr);

            [[noreturn]] var operator + (const var &) override;
            [[noreturn]] var operator - (const var &) override;
            [[noreturn]] var operator * (const var &) override;
            [[noreturn]] var operator / (const var &) override;
            [[noreturn]] var operator % (const var &) override;

            [[noreturn]] var operator >  (const var &) override;
            [[noreturn]] var operator >= (const var &) override;
            [[noreturn]] var operator <  (const var &) override;
            [[noreturn]] var operator <= (const var &) override;
            [[noreturn]] var operator == (const var &) override;
            [[noreturn]] var operator != (const var &) override;

            [[noreturn]] var & operator [] (const var &) override;
            [[noreturn]] var & operator [] (const var &&) override;
            [[noreturn]] var & operator [] (const string::str) override;
        };
    }
}

#endif
