#include "lexer.hh"

void fell::lex::let(std::string & statement) {
    const auto cutoff = statement.find_first_of("=", 4);

    std::size_t i = 4;
    while(!std::isspace(statement[i]) && statement[i] != '=')
        ++i;

    const auto name = statement.substr(4, i - 4);

    check_for_invalid_redefinition(name);

    if(cutoff == std::string::npos) {
        (*lang::global_table)[name] = util::make_var<types::nihil>();
    } else {
        const auto expr = std::string_view{statement.begin() + static_cast<std::int64_t>(cutoff) + 1, statement.end()};
        if(expr.begin() == expr.end())
            throw std::runtime_error{"Expected expression."};

        (*lang::global_table)[name] = solve_expression(expr);
    }
}

void fell::lex::check_for_invalid_redefinition(const std::string & name) {
    if(name == keywords::FALSE || name == keywords::TRUE || name == keywords::LET || name == keywords::NIHIL)
        throw std::runtime_error("Attempt to redefine keyword as different kind of symbol.");
    if(name.find("__string_constant_") == 0)
        throw std::runtime_error("Expression is not assignable.");

    try {
        [[maybe_unused]] const auto s = std::stod(name);
    } catch(...) { return; }

    throw std::runtime_error("Expression is not assignable");
}
