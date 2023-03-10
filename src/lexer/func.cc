#include "lexer.hh"

void fell::lex::solve_func(const std::string_view expr, std::stack<inmemory> & vars, std::size_t & i) {
    ++i;
    std::size_t j = i;

    std::vector<std::string> params;

    if(expr[i] != '|')
        while(i < expr.length()) {
            if(expr[i] == ',') {
                const auto copy{i};

                --i;

                while(std::isspace(expr[i])) --i;

                i += 1;

                while(std::isspace(expr[j])) ++j;

                params.emplace_back(expr.data() + j, i - j);

                i = copy;

                j = copy + 1;
            }

            if(expr[i] == '|') {
                const auto copy{i};

                --i;

                while(std::isspace(expr[i])) --i;

                i += 1;

                while(std::isspace(expr[j])) ++j;
                if(j < i)
                    params.emplace_back(expr.data() + j, i - j);
                i = copy;
                break;
            }

            ++i;
        }

    if(i == expr.length())
        throw std::runtime_error{"Unterminated parameter list."};

    for(auto & par : params)
        for(const auto & c : par)
            if(std::isspace(c))
                throw std::runtime_error{"Extra token in Function definition."};

    while(expr[i] != '{' && i < expr.length())
        ++i;

    if(i == expr.length())
        throw std::runtime_error{"No Function body."};

    ++i;
    j = i;

    std::size_t paren_counter = 1;

    while(paren_counter && i < expr.length()) {
        if(expr[i] == '{')
            ++paren_counter;
        if(expr[i] == '}')
            --paren_counter;
        ++i;
    }

    --i;

    if(expr[i] != '}')
        throw std::runtime_error{"Unterminated Function body."};

    std::string body{expr.data() + j, i - j};

    vars.emplace(util::make_var<types::func>(std::tuple{ params, body, nullptr }));
}
