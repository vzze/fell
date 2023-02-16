#ifndef LEXER_HH
#define LEXER_HH

#include <filesystem>
#include <exception>
#include <stdexcept>
#include <string>
#include <ranges>

#include "file.hh"

namespace fell {
    namespace lex {
        constexpr std::string keywords[] = {
            "let"
        };
        constexpr std::string operators[] = {
            "=",
            "+",
            "-",
            "*",
            "/",
            ">",
            ">=",
            "<",
            "<=",
            "==",
            "!="
        };
        void parse_file(const std::filesystem::path);
    }
}

#endif
