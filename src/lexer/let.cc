#include "lexer.hh"

void fell::lex::let(const std::string & statement) {
    std::size_t i = 4;

    while(!std::isspace(statement[i]) && statement[i] != '=' && i < statement.length())
        ++i;

    const auto name = statement.substr(4, i - 4);

    if(name == "")
        throw std::runtime_error{"Expected name."};

    check_for_invalid_redefinition(name);

    while(i < statement.length() && statement[i] != '=') {
        if(!std::isspace(statement[i]))
            throw std::runtime_error{"Extra token in definition."};

        ++i;
    }

    if(i == statement.length()) {
        (*lang::global_table)[name] = util::make_var<types::nihil>();
    } else {
        const auto expr = std::string_view{statement.begin() + static_cast<std::int64_t>(i) + 1, statement.end()};

        if(expr.begin() == expr.end())
            throw std::runtime_error{"Expected expression."};

        (*lang::global_table)[name] = solve_expression(expr);
    }
}

void fell::lex::check_for_invalid_redefinition(const std::string & name) {
    if(name == keywords::FALSE || name == keywords::TRUE || name == keywords::LET || name == keywords::NIHIL)
        throw std::runtime_error("Attempt to redefine keyword as different kind of symbol.");
    if(name[0] == '"')
        throw std::runtime_error("Expression is not assignable.");

    try {
        [[maybe_unused]] const auto s = std::stod(name);
    } catch(...) { return; }

    throw std::runtime_error("Expression is not assignable");
}
