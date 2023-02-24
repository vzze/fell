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
#include <queue>

#include "variable.hh"
#include "util.hh"

namespace fell {
    namespace lex {
        namespace keywords {
            constexpr std::string RET = "ret";
            constexpr std::string NIHIL = "nihil";
            constexpr std::string TRUE = "true";
            constexpr std::string FALSE = "false";
        }

        struct inmemory {
            types::variable::var non_reference;
            types::variable::var * reference;
            explicit inmemory();
            explicit inmemory(types::variable::var *);
            explicit inmemory(types::variable::var &&);
        };

        extern types::variable::var global_table;
        extern std::vector<std::unordered_map<std::string, inmemory>> contexts;

        void parse_file(const std::filesystem::path);
        void eval_code(const std::string &);

        std::size_t operator_precedence(const std::string_view);
        void apply_operation(const inmemory &&, const inmemory &&, const std::string_view, std::stack<inmemory> &);
        types::variable::var check_for_constant_expression(const std::string_view);

        void solve_func(const std::string_view, std::stack<inmemory> &, std::size_t &);

        void solve_string(const std::string_view, std::stack<inmemory> &, std::size_t &);

        void solve_table(const std::string_view, std::stack<inmemory> &, std::size_t &);
        void solve_table_member(types::variable::var &, const std::string_view);

        void solve_variable(const std::string_view, std::stack<inmemory> &, std::size_t &, bool &);

        void solve_expression_stacks(std::stack<inmemory> &, std::queue<inmemory> &, std::stack<std::string_view> &);
        std::size_t solve_expression(const std::string_view, types::variable::var * = nullptr, std::string_view = "");
    }
}

#endif
