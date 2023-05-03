#include <debug.hh>

#define CASE(arg) \
    case arg: \
        std::clog << #arg << ' '; \
    break; \

#define OUTPUT_DATA(arg, func, ln_delim) \
    for(std::size_t count = 0; const auto & i : arg) { \
        if(count % 5 == 0) std::clog << ln_delim; \
        func; \
        ++count; \
    } \
    std::clog << '\n' \

void fell::debug::scanner(const scan::scanned & data) {
    std::clog << "IDENTIFIERS:";
    OUTPUT_DATA(data.identifiers, std::clog << i << ' ', "\n  ");

    std::clog << "CONSTANTS:\n";

    std::clog << "  INTEGERS:";
    OUTPUT_DATA(data.constant_integers, std::clog << i << ' ', "\n    ");
    std::clog << "  FLOATS:";
    OUTPUT_DATA(data.constant_floatings, std::clog << i << ' ', "\n    ");
    std::clog << "  STRINGS:";
    OUTPUT_DATA(data.constant_strings, std::clog << std::quoted(i) << ' ', "\n    ");

    using enum scan::TOKENS;

    decltype(scan::location::line) current = 1;

    std::clog << '\n' << std::setfill('0') << std::setw(5) << current << ": ";

    for(std::size_t i = 0; i < data.tokens.size(); ++i) {
        if(current < data.locations[i].line) {
            current = data.locations[i].line;
            std::clog << '\n';
            std::clog << std::setfill('0') << std::setw(5) << current << ": ";
        }

        switch(data.tokens[i]) {
            CASE(LEFT_PAREN);
            CASE(RIGHT_PAREN);
            CASE(LEFT_SQUARE);
            CASE(RIGHT_SQUARE);
            CASE(LEFT_CURLY);
            CASE(RIGHT_CURLY);

            CASE(DOT);
            CASE(STAR);
            CASE(SLASH);
            CASE(MODULO);
            CASE(PLUS);
            CASE(MINUS);
            CASE(COMMA);

            CASE(SEMICOLON);

            CASE(BANG);
            CASE(BANG_EQUAL);
            CASE(EQUAL);
            CASE(EQUAL_EQUAL);
            CASE(GREATER);
            CASE(GREATER_EQUAL);
            CASE(LESS);
            CASE(LESS_EQUAL);

            CASE(IDENTIFIER);
            CASE(INTEGER);
            CASE(NUMBER);
            CASE(STRING);
            CASE(NIHIL);
            CASE(FALSE);
            CASE(TRUE);

            CASE(BAR);
            CASE(AND);
            CASE(OR);
            CASE(IF);
            CASE(ELSE);
            CASE(RETURN);
            CASE(FOR);
            CASE(WHILE);
            CASE(DO);

            CASE(EXPOSED);

            CASE(END);
        }
    }
}

#undef CASE
