#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <unordered_map>
#include <type_traits>
#include <functional>
#include <cstdint>
#include <variant>
#include <vector>
#include <string>
#include <cmath>

namespace fell::lib {
    struct params;
}

namespace fell {
    struct var {
        public:
            using integer = std::int64_t;
            using number  = double;
            using string  = std::string;
            using object  = std::pair<std::unordered_map<string, var>, std::vector<var>>;
            using nihil   = char;
            using func    = std::variant<std::size_t, std::function<var(lib::params)>>;

            enum class TYPE : std::int32_t {
                INTEGER,
                NUMBER,
                STRING,
                OBJECT,
                NIHIL,
                FUNCTION
            };
        private:
            using storage = std::variant<integer, number, string, object, nihil, func>;

            template<typename T, typename ... Args>
            struct is_same_multiple {
                static constexpr bool value = (std::is_same_v<T, Args> || ...);
            };

            template<typename T, typename ... Args>
            inline static constexpr bool is_same_multiple_v = is_same_multiple<T, Args...>::value;

            TYPE type;
            storage value;
        public:
            var(const integer &);
            var(const number &);
            var(const string &);
            var(const object &);
            var(const func &);
            var(const nihil &);

            var(integer &&);
            var(number &&);
            var(string &&);
            var(object &&);
            var(func &&);
            var(nihil && = nihil{});

            operator bool() const;

            var operator * (const var &) const;
            var operator / (const var &) const;
            var operator % (const var &) const;
            var operator + (const var &) const;
            var operator - (const var &) const;

            var operator > (const var &) const;
            var operator < (const var &) const;
            var operator >= (const var &) const;
            var operator <= (const var &) const;
            var operator == (const var &) const;
            var operator != (const var &) const;

            var operator && (const var &) const;
            var operator || (const var &) const;

            var * operator [] (const var &);

            template<typename VarType>
            VarType & get() requires(
                is_same_multiple_v<
                    VarType, integer, number, string, object, nihil, func
                >
            ) {
                return std::get<VarType>(value);
            }

            template<typename VarType>
            const VarType & get() const requires(
                is_same_multiple_v<
                    VarType, integer, number, string, object, nihil, func
                >
            ) {
                return std::get<VarType>(value);
            }

            TYPE get_type() const;
    };
}

#endif
