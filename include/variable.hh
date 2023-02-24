#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <unordered_map>
#include <functional>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <tuple>
#include <any>

namespace fell {
    namespace lex {
        struct inmemory;
    }

    namespace api {
        struct params;
    }

    namespace types {
        struct variable {
            // shorthand for std::unique_ptr<variable>
            using var = std::unique_ptr<variable>;

            std::any value;

            variable(std::any);

            [[nodiscard]] virtual var operator + (const variable *) = 0;
            [[nodiscard]] virtual var operator - (const variable *) = 0;
            [[nodiscard]] virtual var operator * (const variable *) = 0;
            [[nodiscard]] virtual var operator / (const variable *) = 0;
            [[nodiscard]] virtual var operator % (const variable *) = 0;

            [[nodiscard]] virtual var operator >  (const variable *) = 0;
            [[nodiscard]] virtual var operator >= (const variable *) = 0;
            [[nodiscard]] virtual var operator <  (const variable *) = 0;
            [[nodiscard]] virtual var operator <= (const variable *) = 0;
            [[nodiscard]] virtual var operator == (const variable *) = 0;
            [[nodiscard]] virtual var operator != (const variable *) = 0;

            [[nodiscard]] virtual var & operator [] (const variable *) = 0;
            [[nodiscard]] virtual var & operator [] (const std::string) = 0;
            [[nodiscard]] virtual var call (std::vector<lex::inmemory> &&) = 0;

            virtual ~variable();
        };

        struct number : public variable {
            // underlying type stored in member value
            using num = double;
            number(num = 0);

            [[nodiscard]] var operator + (const variable *) override;
            [[nodiscard]] var operator - (const variable *) override;
            [[nodiscard]] var operator * (const variable *) override;
            [[nodiscard]] var operator / (const variable *) override;
            [[nodiscard]] var operator % (const variable *) override;

            [[nodiscard]] var operator >  (const variable *) override;
            [[nodiscard]] var operator >= (const variable *) override;
            [[nodiscard]] var operator <  (const variable *) override;
            [[nodiscard]] var operator <= (const variable *) override;
            [[nodiscard]] var operator == (const variable *) override;
            [[nodiscard]] var operator != (const variable *) override;

            [[noreturn]] var & operator [] (const variable *) override;
            [[noreturn]] var & operator [] (const std::string) override;

            [[noreturn]] virtual var call (std::vector<lex::inmemory> &&) override;
        };

        struct string : public variable {
            // underlying type stored in member value
            using str = std::string;
            string(str = "");

            [[nodiscard]] var operator + (const variable *) override;
            [[noreturn]] var operator - (const variable *) override;
            [[noreturn]] var operator * (const variable *) override;
            [[noreturn]] var operator / (const variable *) override;
            [[noreturn]] var operator % (const variable *) override;

            [[nodiscard]] var operator >  (const variable *) override;
            [[nodiscard]] var operator >= (const variable *) override;
            [[nodiscard]] var operator <  (const variable *) override;
            [[nodiscard]] var operator <= (const variable *) override;
            [[nodiscard]] var operator == (const variable *) override;
            [[nodiscard]] var operator != (const variable *) override;

            [[noreturn]] var & operator [] (const variable *) override;
            [[noreturn]] var & operator [] (const str) override;

            [[noreturn]] virtual var call (std::vector<lex::inmemory> &&) override;
        };

        struct table : public variable {
            // underlying type stored in member value
            using tbl = std::unordered_map<string::str, var>*;

            table(tbl = new std::unordered_map<string::str, var>());

            [[noreturn]] var operator + (const variable *) override;
            [[noreturn]] var operator - (const variable *) override;
            [[noreturn]] var operator * (const variable *) override;
            [[noreturn]] var operator / (const variable *) override;
            [[noreturn]] var operator % (const variable *) override;

            [[noreturn]] var operator >  (const variable *) override;
            [[noreturn]] var operator >= (const variable *) override;
            [[noreturn]] var operator <  (const variable *) override;
            [[noreturn]] var operator <= (const variable *) override;
            [[noreturn]] var operator == (const variable *) override;
            [[noreturn]] var operator != (const variable *) override;

            [[nodiscard]] var & operator [] (const variable *) override;
            [[nodiscard]] var & operator [] (const string::str) override;

            [[noreturn]] virtual var call (std::vector<lex::inmemory> &&) override;

            ~table();
        };

        struct nihil : public variable {
            // underlying type stored in member value
            using nil = std::nullptr_t;
            nihil(nil = nullptr);

            [[noreturn]] var operator + (const variable *) override;
            [[noreturn]] var operator - (const variable *) override;
            [[noreturn]] var operator * (const variable *) override;
            [[noreturn]] var operator / (const variable *) override;
            [[noreturn]] var operator % (const variable *) override;

            [[noreturn]] var operator >  (const variable *) override;
            [[noreturn]] var operator >= (const variable *) override;
            [[noreturn]] var operator <  (const variable *) override;
            [[noreturn]] var operator <= (const variable *) override;
            [[noreturn]] var operator == (const variable *) override;
            [[noreturn]] var operator != (const variable *) override;

            [[noreturn]] var & operator [] (const variable *) override;
            [[noreturn]] var & operator [] (const string::str) override;

            [[noreturn]] virtual var call (std::vector<lex::inmemory> &&) override;
        };

        struct func : public variable {
            // underlying type stored in member value
            using data = std::tuple<std::vector<std::string>, std::string, std::function<variable::var(api::params)>>;
            func(data = {});

            [[noreturn]] var operator + (const variable *) override;
            [[noreturn]] var operator - (const variable *) override;
            [[noreturn]] var operator * (const variable *) override;
            [[noreturn]] var operator / (const variable *) override;
            [[noreturn]] var operator % (const variable *) override;

            [[noreturn]] var operator >  (const variable *) override;
            [[noreturn]] var operator >= (const variable *) override;
            [[noreturn]] var operator <  (const variable *) override;
            [[noreturn]] var operator <= (const variable *) override;
            [[noreturn]] var operator == (const variable *) override;
            [[noreturn]] var operator != (const variable *) override;

            [[noreturn]] var & operator [] (const variable *) override;
            [[noreturn]] var & operator [] (const string::str) override;

            [[nodiscard]] virtual var call (std::vector<lex::inmemory> &&) override;
        };
    }
}

#endif
