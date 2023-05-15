#ifndef COMPILER_HH
#define COMPILER_HH

#include <algorithm>
#include <stack>
#include <list>

#include <scanner.hh>
#include <vm.hh>

namespace fell::compiler {
    struct instructions {
        std::stack<std::size_t> ins;
        const scan::scanned * data;
        vm * ref;

        std::vector<std::int32_t> * top(const bool = false, const std::size_t i = 0);
        void pop();
    };

    enum class CONTEXT : std::int32_t {
        IF,
        ELSE,
        ELSE_IF,
        WHILE,
        FUNCTION,
        DO_WHILE,
        BLOCK
    };

    struct context_solver {
        std::size_t in_func = 0;
        std::stack<CONTEXT> contexts;
        std::stack<std::int32_t> jump_table;
    };

    void process(const scan::scanned &, vm &);

    std::int32_t operator_precedence(const vm::INSTRUCTIONS);

    void binary_operation(
        const vm::INSTRUCTIONS,
        bool &,
        const scan::scanned &,
        std::stack<vm::INSTRUCTIONS> &,
        std::size_t &,
        instructions &
    );

    void constant_integer(
        const scan::scanned &,
        std::size_t &,
        std::size_t &,
        bool &,
        instructions &,
        std::stack<vm::INSTRUCTIONS> &
    );

    void constant_float(
        const scan::scanned &,
        std::size_t &,
        std::size_t &,
        bool &,
        instructions &,
        std::stack<vm::INSTRUCTIONS> &
    );

    void constant_string(
        const scan::scanned &,
        std::size_t &,
        std::size_t &,
        bool &,
        instructions &,
        std::stack<vm::INSTRUCTIONS> &
    );


    void expression(
        const scan::scanned &,
        std::size_t &,
        std::size_t &,
        std::size_t &,
        std::size_t &,
        std::size_t &,
        std::list<
            std::pair<
                std::list<std::unordered_map<std::string, std::int32_t>>,
                std::vector<bool>
            >
        > &,
        context_solver &,
        instructions &,
        scan::TOKENS
    );
}

#endif
