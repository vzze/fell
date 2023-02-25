#include "lexer.hh"

void fell::lex::replace_string_codes(std::string & str) {
    if(str.length() <= 1) return;

    if(str[0] == '\\') {
        switch(str[1]) {
            case '"': str.erase(0, 1); break;
            case 'f': str.erase(0, 1); str[0] = '\f'; break;
            case 'n': str.erase(0, 1); str[0] = '\n'; break;
            case 'r': str.erase(0, 1); str[0] = '\r'; break;
            case 't': str.erase(0, 1); str[0] = '\t'; break;
            case 'v': str.erase(0, 1); str[0] = '\v'; break;
        }
    };

    for(std::size_t i = 1; i < str.length() - 1; ++i) {
        if(str[i] == '\\' && str[i - 1] != '\\') {
            switch(str[i + 1]) {
                case '"': str.erase(i, 1); break;
                case 'f': str.erase(i, 1); str[i] = '\f'; break;
                case 'n': str.erase(i, 1); str[i] = '\n'; break;
                case 'r': str.erase(i, 1); str[i] = '\r'; break;
                case 't': str.erase(i, 1); str[i] = '\t'; break;
                case 'v': str.erase(i, 1); str[i] = '\v'; break;
            }
        };
    }
}
void fell::lex::solve_string(const std::string_view expr, std::stack<inmemory> & vars, std::size_t & i) {
    ++i;

    const std::size_t j = i;

    while(i < expr.length()) {
        if(expr[i] == '"' && expr[i - 1] != '\\')
            break;
        ++i;
    }

    if(i == expr.length())
        throw std::runtime_error{"Unterminated string."};

    auto data = std::string{expr.data() + j, i - j};

    replace_string_codes(data);

    vars.push(inmemory{util::make_var<types::string>(data)});
}
