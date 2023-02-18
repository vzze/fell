#include "lexer.hh"
#include "util.hh"

fell::types::variable::var fell::lex::check_for_constant_expression(const std::string expr) {
    if(expr == keywords::FALSE)
        return util::make_var<fell::types::number>(0);
    else if(expr == keywords::TRUE)
        return util::make_var<fell::types::number>(1);
    else if(expr == keywords::NIHIL)
        return util::make_var<fell::types::nihil>();

    return util::make_var<fell::types::number>(std::stod(expr));
}

fell::types::variable::var fell::lex::solve_expression(const std::string && expr) {
    const auto s = expr.find_first_of("(-+*/");
    switch(expr[s]) {
        case '+':
            {
                const auto trim = util::trim(expr.substr(0, s));

                if(trim == "")
                    throw std::runtime_error{"Extra symbol: +"};

                types::variable::var intermediary;

                try {
                    intermediary = check_for_constant_expression(trim);
                } catch(...) {
                    const auto & ref = (*lang::global_table)[trim];

                    if(ref == nullptr)
                        throw std::runtime_error{"Undefined variable: " + trim};

                    util::override(intermediary, ref);
                }

                return *intermediary + solve_expression(expr.substr(s + 1));
            }
        break;
        case '-':
            {
                const auto trim = util::trim(expr.substr(0, s));

                if(trim == "")
                    throw std::runtime_error{"Extra symbol: -"};

                types::variable::var intermediary;

                try {
                    intermediary = check_for_constant_expression(trim);
                } catch(...) {
                    const auto & ref = (*lang::global_table)[trim];

                    if(ref == nullptr)
                        throw std::runtime_error{"Undefined variable: " + trim};

                    util::override(intermediary, ref);
                }

                return *intermediary - solve_expression(expr.substr(s + 1));
            }
        break;
        case '*':

        break;
        case '/':

        break;
        default:
            {
                const auto trim = util::trim(expr);

                if(trim == "")
                    throw std::runtime_error{"Extra symbol"};

                try {
                    return check_for_constant_expression(trim);
                } catch(...) {
                    types::variable::var v;

                    const auto & ref = (*lang::global_table)[trim];

                    if(ref == nullptr)
                        throw std::runtime_error{std::string{"Undefined variable: "} + util::trim(expr)};

                    fell::util::override(v, ref);

                    return v;
                }
            }
        break;
    }
}
