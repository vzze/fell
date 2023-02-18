#ifndef LEXER_HH
#define LEXER_HH

#include <filesystem>
#include <exception>
#include <stdexcept>
#include <string>
#include <ranges>

#include "util.hh"
#include "lang.hh"
#include "variable.hh"

namespace fell {
    namespace lex {
        namespace keywords {
            constexpr std::string LET = "let";
        }

        void parse_file(const std::filesystem::path);
        types::variable::var solve_expression(std::string);
        void let(std::string &&);
    }
}

#endif
