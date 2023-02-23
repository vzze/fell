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
            constexpr std::string NIHIL = "nihil";
            constexpr std::string TRUE = "true";
            constexpr std::string FALSE = "false";
        }

        struct inmemory {
            types::variable::var non_reference;
            types::variable::var * reference;
            explicit inmemory(types::variable::var *);
            explicit inmemory(types::variable::var &&);
        };

        void parse_file(const std::filesystem::path);

        std::size_t operator_precedence(const std::string_view);
        void apply_operation(const inmemory &&, const inmemory &&, const std::string_view, std::stack<inmemory> &);
        types::variable::var check_for_constant_expression(const std::string_view);

        void solve_string(const std::string_view, std::stack<inmemory> &, std::size_t &);

        void solve_table(const std::string_view, std::stack<inmemory> &, std::size_t &);
        void solve_table_member(types::variable::var &, const std::string_view);

        void solve_variable(const std::string_view, std::stack<inmemory> &, std::size_t &, bool &);

        void solve_expression_stacks(std::stack<inmemory> &, std::stack<std::string_view> &);
        void solve_expression(const std::string_view, types::variable::var * = nullptr, std::string_view = "");
    }
}

#endif
