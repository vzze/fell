#include "util.hh"

void fell::util::check_paren(const std::string & file) {
    std::stack<char> parens;
    for(std::size_t i = 1; const auto & c : file)
        switch(c) {
            case '\n':
                ++i;
            break;
            case '(':
            case '[':
            case '{':
                parens.push(c);
            break;

            case ')':
                if(parens.empty() || parens.top() != '(')
                    throw std::runtime_error("Line: " + std::to_string(i) + " Missmatched paren: ()");
                parens.pop();
            break;
            case ']':
                if(parens.empty() || parens.top() != '[')
                    throw std::runtime_error("Line: " + std::to_string(i) + " Missmatched paren: []");
                parens.pop();
            break;
            case '}':
                if(parens.empty() || parens.top() != '{')
                    throw std::runtime_error("Line: " + std::to_string(i) + " Missmatched paren: {}");
                parens.pop();
            break;
        }

    if(!parens.empty()) {
        switch(parens.top()) {
            case '(':
            case ')':
                throw std::runtime_error("Missmatched paren: ()");
            break;
            case '[':
            case ']':
                throw std::runtime_error("Missmatched paren: []");
            break;
            case '{':
            case '}':
                throw std::runtime_error("Missmatched paren: {}");
            break;
        }
    }
}
