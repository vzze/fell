#include <scanner.hh>

bool fell::scan::token(
    scanned & data,
    const std::string_view candidate,
    const std::size_t line,
    const std::size_t column
) {
    using enum TOKENS;

    const auto current = data.tokens.size();

    if(candidate == "if")
        data.push_token(IF, {line, column});
    else if(candidate == "else")
        data.push_token(ELSE, {line, column});
    else if(candidate == "ret")
        data.push_token(RETURN, {line, column});
    else if(candidate == "for")
        data.push_token(FOR, {line, column});
    else if(candidate == "while")
        data.push_token(WHILE, {line, column});
    else if(candidate == "do")
        data.push_token(DO, {line, column});
    else if(candidate == "false")
        data.push_token(FALSE, {line, column});
    else if(candidate == "true")
        data.push_token(TRUE, {line, column});
    else if(candidate == "nihil")
        data.push_token(NIHIL, {line, column});
    else if(candidate == "exposed")
        data.push_token(EXPOSED, {line, column});

    return current != data.tokens.size();
}
