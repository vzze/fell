#include <scanner.hh>

fell::scan::scanned fell::scan::file(const std::filesystem::path path) {
    std::ifstream handle(path);

    if(!handle)
        throw err::common{"File not found: " + path.string()};

    std::string file{
        std::istreambuf_iterator<char>{handle},
        {}
    };

    scanned data;

    using enum TOKENS;

    std::size_t line = 1, column = 0;

#define EXTRA_EQUAL(token1, token2) \
    switch(file[i + 1]) { \
        case '=': \
            data.push_token(token1, {line, column}); \
            ++i; \
            ++column; \
        break; \
        default: \
            data.push_token(token2, {line, column}); \
        break; \
    } \

    for(std::size_t i = 0; i < file.size(); ++i) {
        ++column;

        if(std::isspace(file[i])) {
            if(file[i] == '\n') {
                ++line;
                column = 0;
            }

            continue;
        } else {
            switch(file[i]) {
                case '#': while(file[i] != '\n' && i < file.size()) ++i; break;
                case '(': data.push_token(LEFT_PAREN, {line, column});   break;
                case ')': data.push_token(RIGHT_PAREN, {line, column});  break;

                case '[': data.push_token(LEFT_SQUARE, {line, column});  break;
                case ']': data.push_token(RIGHT_SQUARE, {line, column}); break;

                case '{': data.push_token(LEFT_CURLY, {line, column});   break;
                case '}': data.push_token(RIGHT_CURLY, {line, column});  break;

                case '.': data.push_token(DOT, {line, column});    break;
                case '*': data.push_token(STAR, {line, column});   break;
                case '/': data.push_token(SLASH, {line, column});  break;
                case '%': data.push_token(MODULO, {line, column}); break;
                case '+': data.push_token(PLUS, {line, column});   break;
                case '-': data.push_token(MINUS, {line, column});  break;
                case ',': data.push_token(COMMA, {line, column});  break;

                case ';': data.push_token(SEMICOLON, {line, column}); break;

                case '|': data.push_token(BAR, {line, column}); break;
                case '&': data.push_token(AND, {line, column}); break;
                case '?': data.push_token(OR, {line, column});  break;

                case '!': EXTRA_EQUAL(BANG_EQUAL, BANG);       break;
                case '=': EXTRA_EQUAL(EQUAL_EQUAL, EQUAL);     break;
                case '>': EXTRA_EQUAL(GREATER_EQUAL, GREATER); break;
                case '<': EXTRA_EQUAL(LESS_EQUAL, LESS);       break;

                case '"': {
                    ++i;
                    const auto j = i;

                    auto opt = string(std::string_view{file.data(), file.data() + file.size()}, i);

                    if(!opt.has_value())
                        throw err::common{
                            line,
                            column,
                            "Unterminated string: " +
                            file.substr(j - 1, file.find_first_of(" \n", j - 1))
                        };

                    column += opt.value().length() + 1;

                    replace_string_codes(opt.value());
                    data.push_constant(opt.value(), {line, column});
                } break;

                default: {
                    if(!std::isalnum(file[i]))
                        throw err::common{
                            line,
                            column,
                            "Unknown token: " +
                            file.substr(i, file.find_first_of(" \n", i))
                        };

                    const auto j = i;

                    if('0' <= file[i] && file[i] <= '9')
                        while((std::isalnum(file[i]) || file[i] == '_' || file[i] == '.') && i < file.size())
                            ++i;
                    else
                        while((std::isalnum(file[i]) || file[i] == '_') && i < file.size())
                            ++i;

                    std::string_view candidate{file.data() + j, file.data() + i};

                    if(token(data, candidate, line, column)) {
                    } else if(candidate[0] == '.' || ('0' <= candidate[0] && candidate[0] <= '9')) {
                        number(data, candidate, line, column);
                    } else {
                        if(data.tokens.size() && data.tokens[data.tokens.size() - 1] == DOT) {
                            data.push_constant({candidate.data(), candidate.size()}, {line, column});
                        } else {
                            data.push_identifier({candidate.data(), candidate.size()}, {line, column});
                        }
                    }

                    --i;

                    column += i - j;
                } break;
            }
        }
    }

    data.push_token(END, {line, column});

    return data;

#undef EXTRA_EQUAL
}
