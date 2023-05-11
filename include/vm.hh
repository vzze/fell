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

            explicit holder(const std::size_t, const TYPE);
            explicit holder(var *);
            explicit holder(const var = var::nihil{}, const TYPE = TYPE::UNINITIALIZED);
        };

        enum class INSTRUCTIONS : std::int32_t {
            ADD = -1,   // +
            SUB = -2,   // -
            DIV = -3,   // /
            MUL = -4,   // *
            MOD = -5,   // %
            MOV = -6,   // =
            MAC = -7,   // member access

            PAR = -8,

            LOC = -9,   // load const
            LOV = -10,  // load var
            LOE = -11,  // load exposed

            NE = -12,   // !=
            EQ = -13,   // ==
            GR = -14,   // >
            GE = -15,   // >=
            LR = -16,   // <
            LE = -17,   // <=

            AN = -18,   // &&
            OR = -19,   // ?

            JMP = -20,  // jump
            JE  = -21,  // conditional jump
            RJMP = -22, // reverse jump

            RET = -23,  // return
            POP = -24,  // resize memory
            RJE = -25,  // reverse conditional jump

            CAL = -26,  // call with needed return value
            PU  = -27,  // push param
            CAN = -28,  // call with unneeded return value
            PRC = -29,  // pre call

            LOF = -30   // load var fast
        };

        std::filesystem::path cwd;

        struct stack {
            private:
                std::vector<holder> data;
                std::size_t _size = 0;
            public:
                void init() {
                    data.resize(10000);
                }

                template<typename T>
                void push(T && arg) {
                    data[_size++] = arg;
                }

                template<typename ... Args>
                void emplace(Args && ... args) {
                    data[_size++] = holder{std::forward<Args>(args)...};
                }

                void pop() {
                    --_size;
                }

                holder & top() {
                    return data[_size - 1];
                }

                std::size_t size() const {
                    return _size;
                }

                bool empty() const {
                    return _size == 0;
                }
        } runtime;

        std::vector<holder> memory;
        std::vector<std::size_t> stack_frame = {};
        std::stack<std::size_t> call_info;

        std::stack<std::tuple<
            std::size_t,
            std::pair<std::vector<scan::location>, std::vector<std::int32_t>> *,
            vm::INSTRUCTIONS,
            std::size_t
        >> program;

        std::pair<std::vector<scan::location>, std::vector<std::int32_t>> main_program;

        var & get(holder &);

        var call(var &, std::vector<var*>);
        var call(std::pair<std::vector<scan::location>, std::vector<std::int32_t>> &);

        void init();

        var run(const std::size_t = 0);

        var main();
    };
}

#endif
