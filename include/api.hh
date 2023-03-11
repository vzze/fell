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
                        "Type must be one of the: param::num, param::str, param::tbl, param::fun, param::nil"
                    );
                    return util::get_value<UnderlyingValue>(v.get());
                }

                types::variable::var & expose();

                friend void fell::api::copy(param & a, const param & b);
        };

        template<typename UnderlyingValue>
        inline UnderlyingValue get_value(const types::variable::var & v) {
            static_assert(
                std::is_same_v<UnderlyingValue, param::num> ||
                std::is_same_v<UnderlyingValue, param::str> ||
                std::is_same_v<UnderlyingValue, param::tbl> ||
                std::is_same_v<UnderlyingValue, param::fun> ||
                std::is_same_v<UnderlyingValue, param::nil>,
                "Type must be one of the: param::num, param::str, param::tbl, param::fun, param::nil"
            );
            return util::get_value<UnderlyingValue>(v.get());
        }

        inline void copy(param & a, const param & b) { util::copy(a.v, b.v); }
        inline void copy_var(types::variable::var & a, const types::variable::var & b) { util::copy(a, b); }

        template<typename Type, typename ... Args>
        inline types::variable::var make_var(Args && ... args) {
            static_assert(
                std::is_same_v<Type, types::number> ||
                std::is_same_v<Type, types::string> ||
                std::is_same_v<Type, types::table> ||
                std::is_same_v<Type, types::func> ||
                std::is_same_v<Type, types::nihil>,
                "Type must be one of the: types::number, types::string, types::table, types::func, types::nihil"
            );
            return util::make_var<Type>(std::forward<Args>(args)...);
        }

        struct params {
            private:
                std::vector<lex::inmemory> pars;
            public:
                params(std::vector<lex::inmemory> &&);
                std::vector<lex::inmemory> & expose();
                std::size_t number_of_params();
                param get_param(std::size_t);
                void  for_each_param(std::function<void(param)>);
        };

        types::variable::var make_func(std::function<types::variable::var(params)>);

        void set_global_var(std::string, types::variable::var);
    }
}

#endif
