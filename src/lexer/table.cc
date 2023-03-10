#include "lexer.hh"

void fell::lex::solve_table_member(types::variable::var & tbl, const std::string_view def) {
    if(def[0] != '"')
        throw std::runtime_error{"Table key should be a String."};

    std::size_t i = 1;

    while(i < def.length()) {
        if(def[i] == '"' && def[i] != '\\')
            break;
        ++i;
    }

    if(i == def.length())
        throw std::runtime_error{"Unterminated string."};


    const auto name = def.substr(1, i - 1);
    ++i;

    while(i < def.length() && def[i] != ':') {
        if(!std::isspace(def[i]))
            throw std::runtime_error{"Extra token in definition."};

        ++i;
    }

    if(i == def.length())
        throw std::runtime_error{"Expected property for key: " + std::string(name)};

    const auto expr = std::string_view{def.begin() + static_cast<std::int64_t>(i) + 1, def.end()};

    if(expr.begin() == expr.end())
        throw std::runtime_error{"Expected property."};

    solve_expression(expr, &(*tbl)[std::string(name)], "=");
}

void fell::lex::solve_table(const std::string_view expr, std::stack<inmemory> & vars, std::size_t & i) {
    ++i;

    auto tbl = util::make_var<types::table>();

    std::size_t paren_counter = 1;

    std::size_t j = i;

    bool triggered = true;
    bool func = false;

    while(paren_counter && i < expr.length()) {
        if(expr[i] == '{')
            ++paren_counter;
        else if(expr[i] == ':' && paren_counter == 1)
            triggered = false;
        else if(expr[i] == '|')
            func = !func;
        else if(expr[i] == '}') {
            --paren_counter;
            if(paren_counter == 0 && !triggered) {
                while(std::isspace(expr[j])) ++j;
                const auto copy = i;
                --i;
                while(std::isspace(expr[i])) --i;
                solve_table_member(tbl, std::string_view{expr.data() + j, i - j + 1});
                i = copy;
            }
        } else if(expr[i] == ',' && paren_counter == 1 && !func) {
            while(std::isspace(expr[j])) ++j;
            const auto copy = i;
            --i;
            while(std::isspace(expr[i])) --i;
            solve_table_member(tbl, std::string_view{expr.data() + j, i - j + 1});
            triggered = true;
            i = copy;
            j = i + 1;
        }
        ++i;
    }

    --i;

    if(expr[i] != '}')
        throw std::runtime_error{"Unterminated Table definition."};

    vars.emplace(std::move(tbl));
}
