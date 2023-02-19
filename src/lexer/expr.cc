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
    const auto s = expr.find_first_of("(-+*/%");

    const auto trim = util::trim(expr.substr(0, s));

    if(trim == "")
        throw std::runtime_error{"Extra symbol."};

    if(trim.find(' ') != std::string::npos)
        throw std::runtime_error("Extra keyword.");

    types::variable::var intermediary;

    try {
        intermediary = check_for_constant_expression(trim);
    } catch(...) {
        const auto & ref = (*lang::global_table)[trim];

        if(ref == nullptr)
            throw std::runtime_error{"Undefined variable: " + trim};

        util::copy(intermediary, ref);
    }

    switch(expr[s]) {
        case '+':
            return *intermediary + solve_expression(expr.substr(s + 1));
        break;
        case '-':
            return *intermediary - solve_expression(expr.substr(s + 1));
        break;
        case '%':
            return *intermediary % solve_expression(expr.substr(s + 1));
        break;
        case '*':

        break;
        case '/':

        break;
        default:
            return intermediary;
        break;
    }
}
