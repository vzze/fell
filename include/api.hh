#ifndef API_HH
#define API_HH

#include <string>
#include <functional>
#include <type_traits>

#include "lexer.hh"
#include "util.hh"
#include "variable.hh"

namespace fell {
    namespace api {
        struct param;

        inline void copy(param & a, const param & b);

        struct param {
            using num = types::number::num;
            using str = types::string::str;
            using tbl = types::table::tbl;
            using fun = types::func::data;
            using nil = types::nihil::nil;

            private:
                types::variable::var & v;
            public:
                param(types::variable::var &);
                template<typename UnderlyingValue>
                UnderlyingValue & get_value() {
                    static_assert(
                        std::is_same_v<UnderlyingValue, num> ||
                        std::is_same_v<UnderlyingValue, str> ||
                        std::is_same_v<UnderlyingValue, tbl> ||
                        std::is_same_v<UnderlyingValue, fun> ||
                        std::is_same_v<UnderlyingValue, nil>,
                        "Value must be one of the: param::num, param::str, param::tbl, param::fun, param::nil"
                    );
                    return util::get_value<UnderlyingValue>(v.get());
                }

                friend void fell::api::copy(param & a, const param & b);
        };

        inline void copy(param & a, const param & b) { util::copy(a.v, b.v); }

        template<typename Type, typename ... Args>
        inline types::variable::var make_var(Args && ... args) {
            return util::make_var<Type>(std::forward<Args>(args)...);
        }

        struct params {
            private:
                std::vector<lex::inmemory> pars;
            public:
                params(std::vector<lex::inmemory> &&);
                param get_param(std::size_t);
                void  for_each_param(std::function<void(param)>);
        };

        types::variable::var make_func(std::function<types::variable::var(params)>);
    }
}

#endif
