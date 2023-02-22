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

        void parse_file(const std::filesystem::path);

        std::size_t operator_precedence(const std::string_view);
        types::variable::var apply_operation(const types::variable::var &&, const types::variable::var &&, const std::string_view);
        types::variable::var check_for_constant_expression(const std::string_view);

        void solve_string(const std::string_view, std::stack<types::variable::var> &, std::size_t &);

        void solve_table(const std::string_view, std::stack<types::variable::var> &, std::size_t &);
        void solve_table_member(types::variable::var &, const std::string_view);

        void solve_variable(const std::string_view, std::stack<types::variable::var> &, std::size_t &, bool &);

        void solve_expression_stacks(std::stack<types::variable::var> &, std::stack<std::string_view> &);
        types::variable::var solve_expression(const std::string_view);

        void let(const std::string_view);
        void check_for_invalid_redefinition(const std::string_view);
    }
}

#endif
