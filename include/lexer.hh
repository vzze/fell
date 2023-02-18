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
            constexpr std::string NIHIL = "nihil";
            constexpr std::string TRUE = "true";
            constexpr std::string FALSE = "false";
        }

        void parse_file(const std::filesystem::path);
        types::variable::var check_for_constant_expression(const std::string);
        types::variable::var solve_expression(const std::string &&);
        void let(std::string &&);
    }
}

#endif
