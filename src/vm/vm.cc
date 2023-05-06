#include <vm.hh>
#include <error.hh>
#include <std.hh>

fell::vm::holder::holder(const std::size_t v, const TYPE t) : type{t}, value{v} {}
fell::vm::holder::holder(var * var) : type{TYPE::OBJ_PROP}, value{var} {}
fell::vm::holder::holder(var var, TYPE t) : type{t}, value{var} {}

std::unordered_map<std::string, fell::var> fell::exposed = {};
std::vector<fell::var> fell::constants;
std::vector<std::pair<std::vector<fell::scan::location>, std::vector<std::int32_t>>> fell::labels;

fell::var & fell::vm::get(holder & h) {
    switch(h.type) {
        case holder::TYPE::REFERENCE:
            while(memory[std::get<std::size_t>(h.value)].type == holder::TYPE::REFERENCE) {
                h.value = std::get<std::size_t>(memory[std::get<std::size_t>(h.value)].value);
            }

            return get(memory[std::get<std::size_t>(h.value)]);
        break;

        case holder::TYPE::CONSTANT:
            return constants[std::get<std::size_t>(h.value)];
        break;

        case holder::TYPE::EXPOSED:
            return exposed[constants[std::get<std::size_t>(h.value)].get<var::string>()];
        break;

        case holder::TYPE::VALUE:
        case holder::TYPE::UNINITIALIZED:
            return std::get<var>(h.value);
        break;

        case holder::TYPE::OBJ_PROP:
            return *std::get<var*>(h.value);
        break;
    }

    throw fell::err::common(0, 0, "Something really bad happened.");
}

void fell::vm::call(const scan::location location, INSTRUCTIONS call_type) {
    using enum INSTRUCTIONS;
    using enum holder::TYPE;

    const auto sz = call_info.top();
    call_info.pop();

    stack_frame.push_back(static_cast<std::int32_t>(memory.size() - sz));

    if(get(runtime.top()).get_type() != var::TYPE::FUNCTION)
        throw err::common(location.line, location.column,  "Attempt to call a non-function variable.");

    if(std::holds_alternative<std::size_t>(get(runtime.top()).get<var::func>())) {
        const auto & value = labels[std::get<std::size_t>(get(runtime.top()).get<var::func>())];
        runtime.pop();

        const auto stack_size = runtime.size();

        run(value);

        if(call_type == CAL) {
            if(runtime.size() == stack_size)
                runtime.emplace(var::nihil{}, VALUE);
            else if(runtime.top().type == REFERENCE) {
                var ne = get(runtime.top());

                runtime.pop();
                runtime.emplace(ne, VALUE);
            }
        } else if(call_type == CAN && runtime.size() != stack_size)
            runtime.pop();
    } else {
        try {
            const auto & fn = std::get<std::function<fell::var(lib::params)>>(get(runtime.top()).get<var::func>());
            runtime.pop();
            var ret_value = fn({ this, static_cast<std::size_t>(stack_frame.back()), sz });

            if(call_type == CAL)
                runtime.emplace(ret_value, VALUE);

        } catch(const std::exception & e) {
            throw err::common(location.line, location.column, e.what());
        }
    }

    memory.resize(static_cast<std::size_t>(stack_frame.back()));
    stack_frame.pop_back();
}

#define mem_loc(i) static_cast<std::size_t>(instructions.second[static_cast<std::size_t>(i) - 1])

#define binary_op(op) \
    try {                                                      \
        switch(runtime.top().type) {                           \
            case VALUE: {                                      \
                const var rhs = get(runtime.top());            \
                runtime.pop();                                 \
                                                               \
                switch(runtime.top().type) {                   \
                    case VALUE: {                              \
                        const var lhs = get(runtime.top());    \
                                                               \
                        runtime.pop();                         \
                                                               \
                        runtime.emplace(lhs op rhs, VALUE);    \
                    } break;                                   \
                                                               \
                    default: {                                 \
                        const auto & lhs = get(runtime.top()); \
                                                               \
                        runtime.pop();                         \
                                                               \
                        runtime.emplace(lhs op rhs, VALUE);    \
                    } break;                                   \
                }                                              \
            } break;                                           \
            default: {                                         \
                const auto & rhs = get(runtime.top());         \
                runtime.pop();                                 \
                                                               \
                switch(runtime.top().type) {                   \
                    case VALUE: {                              \
                        const var lhs = get(runtime.top());    \
                                                               \
                        runtime.pop();                         \
                                                               \
                        runtime.emplace(lhs op rhs, VALUE);    \
                    } break;                                   \
                                                               \
                    default: {                                 \
                        const auto & lhs = get(runtime.top()); \
                                                               \
                        runtime.pop();                         \
                                                               \
                        runtime.emplace(lhs op rhs, VALUE);    \
                    } break;                                   \
                }                                              \
            } break;                                           \
        }                                                      \
    } catch(const std::exception & e) {                        \
        throw err::common(instructions.first[i].line, instructions.first[i].column, e.what()); \
    }                                                          \

void fell::vm::run(const std::pair<std::vector<scan::location>, std::vector<std::int32_t>> & instructions) {
    using enum INSTRUCTIONS;
    using enum holder::TYPE;

    for(std::size_t i = 0; i != instructions.second.size(); ++i) {
        switch(static_cast<INSTRUCTIONS>(instructions.second[i])) {
            case CAL:
            case CAN:
                call(instructions.first[i], static_cast<INSTRUCTIONS>(instructions.second[i]));
            break;

            case PRC:
                call_info.push(0);
            break;

            case PU:
                memory.push_back(runtime.top());
                runtime.pop();

                ++call_info.top();
            break;

            case RET:
                return;
            break;

            case LOC:
                runtime.emplace(mem_loc(i), CONSTANT);
            break;

            case LOV:
                switch(static_cast<vm::INSTRUCTIONS>(mem_loc(i))) {
                    case TOP:
                        if(memory.size() <= mem_loc(i - 1) + static_cast<std::size_t>(stack_frame.back()))
                            memory.resize(mem_loc(i - 1) + static_cast<std::size_t>(stack_frame.back()) + 1);

                        switch(memory[mem_loc(i - 1) + static_cast<std::size_t>(stack_frame.back())].type) {
                            case VALUE:
                                runtime.emplace(
                                        std::get<var>(
                                            memory[
                                                mem_loc(i - 1) + static_cast<std::size_t>(stack_frame.back())
                                            ].value
                                        ),
                                    VALUE
                                );
                            break;

                            default:
                                runtime.emplace(mem_loc(i - 1) + static_cast<std::size_t>(stack_frame.back()), REFERENCE);
                            break;
                        }
                    break;

                    default:
                        if(mem_loc(i) >= stack_frame.size()) throw err::common(instructions.first[i].line, instructions.first[i].column, "Attempt to access expired variable.");

                        if(memory.size() <= mem_loc(i - 1) + static_cast<std::size_t>(stack_frame[mem_loc(i)]))
                            memory.resize(mem_loc(i - 1) + static_cast<std::size_t>(stack_frame[mem_loc(i)]) + 1);

                        switch(memory[mem_loc(i - 1) + static_cast<std::size_t>(stack_frame[mem_loc(i)])].type) {
                            case VALUE:
                                runtime.emplace(
                                        std::get<var>(
                                            memory[
                                                mem_loc(i - 1) + static_cast<std::size_t>(stack_frame[mem_loc(i)])
                                            ].value
                                        ),
                                    VALUE
                                );
                            break;

                            default:
                                runtime.emplace(mem_loc(i - 1) + static_cast<std::size_t>(stack_frame[mem_loc(i)]), REFERENCE);
                            break;
                        }

                    break;
                }
            break;

            case LOE:
                runtime.emplace(mem_loc(i), EXPOSED);
            break;

            case POP:
                memory.resize(memory.size() - mem_loc(i));
            break;

            case MAC: {
                switch(runtime.top().type) {
                    case VALUE: {
                        const auto rhs = get(runtime.top());
                        runtime.pop();

                        switch(runtime.top().type) {
                            case VALUE: {
                                var lhs = get(runtime.top());

                                runtime.pop();

                                runtime.emplace(*lhs[rhs], VALUE);
                            } break;

                            default: {
                                auto & lhs = get(runtime.top());

                                runtime.pop();

                                runtime.emplace(lhs[rhs]);
                            } break;
                        }
                    } break;
                    default: {
                        const auto & rhs = get(runtime.top());
                        runtime.pop();

                        switch(runtime.top().type) {
                            case VALUE: {
                                var lhs = get(runtime.top());

                                runtime.pop();

                                runtime.emplace(*lhs[rhs], VALUE);
                            } break;

                            default: {
                                auto & lhs = get(runtime.top());

                                runtime.pop();

                                runtime.emplace(lhs[rhs]);
                            } break;
                        }
                    } break;
                }
            } break;

            case JE:
                if(runtime.empty()) throw err::common(instructions.first[i].line, instructions.first[i].column, "Malformed statement.");

                if(!(bool)(get((runtime.top())))) i += mem_loc(i) - 1;

                runtime.pop();
            break;

            case JMP:
                i += mem_loc(i) - 1;
            break;

            case RJMP:
                i -= mem_loc(i);
            break;

            case RJE:
                if((bool)(get(runtime.top()))) i -= mem_loc(i);

                runtime.pop();
            break;

            case ADD: binary_op(+); break;
            case SUB: binary_op(-); break;
            case DIV: binary_op(/); break;
            case MUL: binary_op(*); break;
            case MOD: binary_op(%); break;

            case AN: binary_op(&&); break;
            case OR: binary_op(||); break;

            case NE: binary_op(!=); break;
            case EQ: binary_op(==); break;

            case GR: binary_op(>); break;
            case GE: binary_op(>=); break;
            case LR: binary_op(<); break;
            case LE: binary_op(<=); break;

            case MOV: {
                switch(runtime.top().type) {
                    case VALUE: {
                        const var rhs = get(runtime.top());
                        runtime.pop();

                        switch(runtime.top().type) {
                            case VALUE:
                                throw err::common(instructions.first[i].line, instructions.first[i].column, "Attempt to initialize a temporary value.");
                            break;

                            case CONSTANT:
                                throw err::common(instructions.first[i].line, instructions.first[i].column, "Attempt to initialize a constant value");
                            break;

                            default: {
                                auto & lhs = get(runtime.top());

                                runtime.pop();

                                lhs = rhs;
                            } break;
                        }
                    } break;
                    default: {
                        const auto & rhs = get(runtime.top());
                        runtime.pop();

                        switch(runtime.top().type) {
                            case VALUE:
                                throw err::common(instructions.first[i].line, instructions.first[i].column, "Attempt to initialize a temporary value.");
                            break;

                            case CONSTANT:
                                throw err::common(instructions.first[i].line, instructions.first[i].column, "Attempt to initialize a constant value");
                            break;

                            default: {
                                auto & lhs = get(runtime.top());

                                runtime.pop();

                                lhs = rhs;
                            } break;
                        }
                    } break;
                }
            } break;

            default: break;
        }
    }

    throw err::common(instructions.first.back().line, instructions.first.back().column, "Malformed bytecode");
}

#undef mem_loc
#undef binary_op
