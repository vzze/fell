#include <scanner.hh>

void fell::scan::replace_string_codes(var::string & str) {
    if(str.length() <= 1)
        return;

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

std::optional<fell::var::string> fell::scan::string(const std::string_view str, std::size_t & i) {
    const auto j = i;

    while(i < str.length()) {
        if(str[i] == '\n')
            return {};

        if(str[i] == '"' && str[i - 1] != '\\')
            break;

        ++i;
    }

    if(i == str.length())
        return {};

    const auto data = var::string{str.data() + j, i - j};

    return data;
}
