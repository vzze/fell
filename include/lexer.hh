#ifndef LEXER_HH
#define LEXER_HH

#include <string_view>
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <ranges>
#include <cctype>
#include <stack>

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

        std::size_t check_for_string_constant(std::string &);
        void clear_string_constants(std::size_t);
        void parse_file(const std::filesystem::path);

        std::size_t operator_precedence(const std::string &);
        types::variable::var apply_operation(const types::variable::var &&, const types::variable::var &&, const std::string &&);
        types::variable::var check_for_constant_expression(const std::string &);
        void solve_variable(const std::string_view &, std::stack<types::variable::var> &, std::size_t &);
        types::variable::var solve_expression(const std::string_view);

        void let(std::string &);
        void check_for_invalid_redefinition(const std::string &);
    }
}

#endif
