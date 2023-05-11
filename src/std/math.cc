#include <std.hh>

#define one_arg_func_no_conv(name) \
    {                                                                                \
        #name,                                                                       \
        var::func{[](params params) -> var {                                         \
            switch(params[0].get_type()) {                                           \
                case var::TYPE::INTEGER:                                             \
                    return var::integer{std::name(params[0].get<var::integer>())};   \
                break;                                                               \
                                                                                     \
                case var::TYPE::NUMBER:                                              \
                    return var::number{std::name(params[0].get<var::number>())};     \
                break;                                                               \
                                                                                     \
                default:                                                             \
                    throw err::common("Expected Number/Integer");                    \
                break;                                                               \
            }                                                                        \
        }}                                                                           \
    }                                                                                \

#define one_arg_func_conv(name) \
    {                                                                                \
        #name,                                                                       \
        var::func{[](params params) -> var {                                         \
            switch(params[0].get_type()) {                                           \
                case var::TYPE::INTEGER:                                             \
                    return var::number{std::name(params[0].get<var::integer>())};    \
                break;                                                               \
                                                                                     \
                case var::TYPE::NUMBER:                                              \
                    return var::number{std::name(params[0].get<var::number>())};     \
                break;                                                               \
                                                                                     \
                default:                                                             \
                    throw err::common("Expected Number/Integer");                    \
                break;                                                               \
            }                                                                        \
        }}                                                                           \
    }                                                                                \

std::unordered_map<fell::var::string, fell::var> fell::lib::math() {
    return {
        {
            "pi",
            var::number{std::numbers::pi}
        },
        {
            "huge",
            var::number{std::numeric_limits<var::number>::max()}
        },
        {
            "tiny",
            var::number{std::numeric_limits<var::number>::min()}
        },
        {
            "maxinteger",
            var::integer{std::numeric_limits<var::integer>::max()}
        },
        {
            "mininteger",
            var::integer{std::numeric_limits<var::integer>::min()}
        },
        {
            "random",
            var::func{[]([[maybe_unused]] params params) -> var {
                static std::random_device rd;
                static std::mt19937_64 mersenne{rd()};

                return static_cast<var::integer>(mersenne());
            }}
        },
        {
            "max",
            var::func{[](params params) -> var {
                var max{std::numeric_limits<var::integer>::min()};

                params.for_each([&](const var & par) -> void {
                    if(par > max) max = par;
                });

                return max;
            }}
        },
        {
            "min",
            var::func{[](params params) -> var {
                var min{std::numeric_limits<var::number>::max()};

                params.for_each([&](const var & par) -> void {
                    if(par < min) min = par;
                });

                return min;
            }}
        },
        {
            "frexp",
            var::func{[](params params) -> var {
                using enum var::TYPE;
                std::int32_t exp;
                switch(params[0].get_type()) {
                    case INTEGER: {
                        const var::number m = std::frexp(params[0].get<var::integer>(), &exp);
                        return var::object{
                            {
                                { "tails", m },
                                { "exponent", var::integer{exp}}
                            },
                            {}
                        };
                    } break;

                    case NUMBER: {
                        const var::number m = std::frexp(params[0].get<var::number>(), &exp);
                        return var::object{
                            {
                                { "tails", m },
                                { "exponent", var::integer{exp}}
                            },
                            {}
                        };
                    } break;

                    default:
                        throw err::common("Expected Number/Integer");
                    break;
                }
            }}
        },
        {
            "ldexp",
            var::func{[](params params) -> var {
                return std::ldexp(
                    params[0][var::string{"tails"}]->get<var::number>(),
                    static_cast<std::int32_t>(
                        params[0][var::string{"exponent"}]->get<var::integer>()
                    )
                );
            }}
        },
        {
            "modf",
            var::func{[](params params) -> var {
                var::number num;
                return var::object{
                    {
                        {
                            "fractional",
                            var::number{std::modf(params[0].get<var::number>(), &num)}},
                        {
                            "integral",
                            num
                        }
                    },
                    {}
                };
            }}
        },
        {
            "pow",
            var::func{[](params params) -> var {
                using enum var::TYPE;
                switch(params[0].get_type()) {
                    case INTEGER:
                        switch(params[1].get_type()) {
                            case INTEGER:
                                return var::number{
                                    std::pow(
                                        params[0].get<var::integer>(),
                                        params[1].get<var::integer>()
                                    )
                                };
                            break;

                            case NUMBER:
                                return var::number{
                                    std::pow(
                                        params[0].get<var::integer>(),
                                        params[1].get<var::number>()
                                    )
                                };
                            break;

                            default:
                                throw err::common("Expected Number/Integer");
                            break;
                        }
                    break;

                    case NUMBER:
                        switch(params[1].get_type()) {
                            case INTEGER:
                                return var::number{
                                    std::pow(
                                        params[0].get<var::number>(),
                                        params[1].get<var::integer>()
                                    )
                                };
                            break;

                            case NUMBER:
                                return var::number{
                                    std::pow(
                                        params[0].get<var::number>(),
                                        params[1].get<var::number>()
                                    )
                                };
                            break;

                            default:
                                throw err::common("Expected Number/Integer");
                            break;
                        }
                    break;

                    default:
                        throw err::common("Expected Number/Integer");
                    break;
                }
            }}
        },
        one_arg_func_no_conv(abs),
        one_arg_func_conv(acos),
        one_arg_func_conv(asin),
        one_arg_func_conv(ceil),
        one_arg_func_conv(cos),
        one_arg_func_conv(cosh),
        one_arg_func_conv(exp),
        one_arg_func_conv(floor),
        one_arg_func_conv(log10),
        one_arg_func_conv(sin),
        one_arg_func_conv(sinh),
        one_arg_func_conv(sqrt),
        one_arg_func_conv(tan),
        one_arg_func_conv(tanh),
        one_arg_func_conv(atan)
    };
}

#undef one_arg_func_no_conv
#undef one_arg_func_conv
