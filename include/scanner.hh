#ifndef SCANNER_HH
#define SCANNER_HH

#include <string_view>
#include <filesystem>
#include <optional>
#include <fstream>
#include <cstdint>
#include <string>

#include <variable.hh>
#include <error.hh>

namespace fell::scan {
    enum class TOKENS : std::int32_t {
        LEFT_PAREN, RIGHT_PAREN,   // ()
        LEFT_SQUARE, RIGHT_SQUARE, // []
        LEFT_CURLY, RIGHT_CURLY,   // {}

        DOT,    // .
        STAR,   // *
        SLASH,  // /
        MODULO, // %
        PLUS,   // +
        MINUS,  // -
        COMMA,  // ,

        SEMICOLON, // ;

        BANG,                   // !
        BANG_EQUAL,             // !=
        EQUAL,                  // =
        EQUAL_EQUAL,            // ==
        GREATER, GREATER_EQUAL, // > >=
        LESS, LESS_EQUAL,       // < <=

        IDENTIFIER, // variable name
        INTEGER,    // compile time integer
        NUMBER,     // compile time number
        STRING,     // compile time string
        NIHIL,
        FALSE,
        TRUE,

        BAR,    // |
        AND,    // &
        OR,     // ?
        IF,     // if
        ELSE,   // else
        RETURN, // ret
        FOR,    // for
        WHILE,  // while
        DO,     // do { ... } while

        EXPOSED,

        END // if <fstream> is included i cant name it EOF ??
    };

    struct location {
        std::size_t line, column;
    };

    struct scanned {
        std::vector<location> locations;
        std::vector<var::string> identifiers;
        std::vector<TOKENS> tokens;

        std::vector<var::integer> constant_integers;
        std::vector<var::number>  constant_floatings;
        std::vector<var::string>  constant_strings;

        void push_token(const TOKENS, const location);
        void push_identifier(const var::string, const location);

        void push_constant(const var::integer, const location);
        void push_constant(const var::number, const location);
        void push_constant(const var::string, const location);
    };

    scanned file(const std::filesystem::path);

    bool token(scanned & , const std::string_view, const std::size_t, const std::size_t);
    void number(scanned &, const std::string_view, const std::size_t, const std::size_t);

    std::optional<var::string> string(const std::string_view, std::size_t &);
    void replace_string_codes(var::string &);
}

#endif
