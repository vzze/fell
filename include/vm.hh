#ifndef VM_HH
#define VM_HH

#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <stack>

#include <variable.hh>
#include <scanner.hh>

namespace fell {
    extern std::unordered_map<std::string, var> exposed;
    extern std::vector<var> constants;
    extern std::vector<std::pair<std::vector<scan::location>, std::vector<std::int32_t>>> labels;

    struct vm {
        struct holder {
            enum class TYPE : std::int8_t {
                REFERENCE     = 0,
                VALUE         = 1,
                CONSTANT      = 2,
                EXPOSED       = 3,
                OBJ_PROP      = 4,
                UNINITIALIZED = 5,
            };

            TYPE type;
            std::variant<std::size_t, var, var*> value;
            holder(const std::size_t, const TYPE);
            holder(var *);
            holder(const var = var::nihil{}, const TYPE = TYPE::UNINITIALIZED);
        };

        enum class INSTRUCTIONS : std::int32_t {
            ADD = -1, // +
            SUB = -2, // -
            DIV = -3, // /
            MUL = -4, // *
            MOD = -5, // %
            MOV = -6, // =
            MAC = -7, // [ member access

            PAR = -8,

            LOC = -9, // load const
            LOV = -10, // load var
            LOE = -11, // load exposed

            NE = -12, // !=
            EQ = -13, // ==
            GR = -14, // >
            GE = -15, // >=
            LR = -16, // <
            LE = -17, // <=

            AN = -18, // &&
            OR = -19, // ?

            JMP = -20,
            JE  = -21,
            RJMP = -22,

            RET = -23,
            POP = -24,
            RJE = -25,

            CAL = -26,
            PU  = -27,
            CAN = -28,

            TOP = -1602
        };

        std::stack<holder> runtime;
        std::vector<std::int32_t> stack_frame = { 0 };
        std::filesystem::path cwd;

        std::vector<holder> memory;

        std::pair<std::vector<scan::location>, std::vector<std::int32_t>> ins;

        var & get(holder &);

        void call(const scan::location, INSTRUCTIONS);

        std::stack<std::stack<std::size_t>> call_info;

        void run(const std::pair<std::vector<scan::location>, std::vector<std::int32_t>> &);
    };
}

#endif
