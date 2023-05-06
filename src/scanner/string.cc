#include <scanner.hh>

void fell::scan::replace_string_codes(var::string & str, const std::size_t line, const std::size_t column) {
    if(str.length() <= 1)
        return;

    for(std::size_t i = 0; i < str.length() - 1; ++i) {
        if(str[i] == '\\') {
            switch(str[i + 1]) {
                case '\\': str.erase(i, 1); break;
                case '"':  str.erase(i, 1); break;
                case 'f':  str.erase(i, 1); str[i] = '\f'; break;
                case 'n':  str.erase(i, 1); str[i] = '\n'; break;
                case 'r':  str.erase(i, 1); str[i] = '\r'; break;
                case 't':  str.erase(i, 1); str[i] = '\t'; break;
                case 'v':  str.erase(i, 1); str[i] = '\v'; break;
                default:
                    throw err::common(line, column, std::string("Unknown escaped character: \\") + str[i + 1]);
                break;
            }
        };
    }
}

std::optional<fell::var::string> fell::scan::string(const std::string_view str, std::size_t & i) {
    const auto j = i;

    while(i < str.length()) {
        if(str[i] == '\n')
            return {};

        if((str[i] == '"' && str[i - 1] != '\\') || (str[i] == '"' && str[i - 1] == '\\' && str[i - 2] == '\\'))
            break;

        ++i;
    }

    if(i == str.length())
        return {};

    const auto data = var::string{str.data() + j, i - j};

    return data;
}
