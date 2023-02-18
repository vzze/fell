#include "lexer.hh"
#include "util.hh"

fell::types::variable::var fell::lex::check_for_constant_expression(const std::string && expr) {
    return util::make_var<fell::types::number>(std::stod(expr));
}

fell::types::variable::var fell::lex::solve_expression(const std::string && expr) {
    const auto s = expr.find_first_of("(-+*/");
    switch(expr[s]) {
        case '+':
            try {
                return *check_for_constant_expression(util::trim(expr.substr(0, s))) + solve_expression(expr.substr(s + 1));
            } catch(...) {
                const auto & ref = (*lang::global_table)[util::trim(expr.substr(0, s))];
                if(ref == nullptr)
                    throw std::runtime_error{std::string{"Undefined variable: "} + util::trim(expr)};

                return *ref + solve_expression(expr.substr(s + 1));
            }
        break;
        case '-':
            try {
                return *check_for_constant_expression(util::trim(expr.substr(0, s))) - solve_expression(expr.substr(s + 1));
            } catch(...) {
                const auto & ref = (*lang::global_table)[util::trim(expr.substr(0, s))];
                if(ref == nullptr)
                    throw std::runtime_error{std::string{"Undefined variable: "} + util::trim(expr)};

                return *ref - solve_expression(expr.substr(s + 1));
            }
        break;
        case '*':

        break;
        case '/':

        break;
        default:
            try {
                return check_for_constant_expression(util::trim(expr));
            } catch(...) {
                types::variable::var v;
                const auto & ref = (*lang::global_table)[util::trim(expr)];
                if(ref == nullptr)
                    throw std::runtime_error{std::string{"Undefined variable: "} + util::trim(expr)};

                fell::util::override(v, ref);
                return v;
            }
        break;
    }
}
