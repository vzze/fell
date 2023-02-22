#include "lexer.hh"

void fell::lex::solve_string(const std::string_view expr, std::stack<types::variable::var> & vars, std::size_t & i) {
    ++i;

    const std::size_t j = i;

    while(i < expr.length()) {
        if(expr[i] == '"' && expr[i] != '\\')
            break;
        ++i;
    }

    if(i == expr.length())
        throw std::runtime_error{"Unterminated string."};

    vars.push(util::make_var<types::string>(std::string{expr.data() + j, i - j}));
}
