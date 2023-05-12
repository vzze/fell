#include <vm.hh>
#include <error.hh>
#include <std.hh>

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
        throw err::common((*locations)[*index].line, (*locations)[*index].column, e.what()); \
    }                                                          \

#define mem_loc(i) static_cast<std::size_t>((*instructions)[i - 1])

fell::var fell::vm::main() {
    init();
    return run();
}

void fell::vm::init() {
    runtime.init();
    program.push({0, &main_program, vm::INSTRUCTIONS::CAL, 0});

    stack_frame.push_back(0);
}

fell::var fell::vm::run(const std::size_t stopping_point) {
    using enum INSTRUCTIONS;
    using enum holder::TYPE;

    auto * index              = &std::get<0>(program.top());
    auto * instructions       = &std::get<1>(program.top())->second;
    auto * locations          = &std::get<1>(program.top())->first;
    auto * program_call_type  = &std::get<2>(program.top());
    auto * program_stack_size = &std::get<3>(program.top());

    while(*index < instructions->size()) {
        switch(static_cast<INSTRUCTIONS>((*instructions)[*index])) {
            case CAN: [[fallthrough]];
            case CAL:
                stack_frame.push_back(memory.size() - call_info.top());
                call_info.pop();

                if(get(runtime.top()).get_type() != var::TYPE::FUNCTION)
                    throw err::common((*locations)[(*index)].line, (*locations)[*index].column,  "Attempt to call a non-function variable.");

                if(std::holds_alternative<std::size_t>(get(runtime.top()).get<var::func>())) {
                    auto & value = labels[std::get<std::size_t>(get(runtime.top()).get<var::func>())];
                    runtime.pop();

                    program.push({0, &value, static_cast<INSTRUCTIONS>((*instructions)[*index]), runtime.size()});

                    auto & top = program.top();

                    index              = &std::get<0>(top);
                    instructions       = &std::get<1>(top)->second;
                    locations          = &std::get<1>(top)->first;
                    program_call_type  = &std::get<2>(top);
                    program_stack_size = &std::get<3>(top);
                    continue;
                } else {
                    try {
                        const auto & fn = std::get<std::function<fell::var(lib::params)>>(get(runtime.top()).get<var::func>());
                        runtime.pop();

                        var ret_value = fn({
                            this,
                            stack_frame.back(), memory.size() - stack_frame.back()
                        });

                        if(static_cast<INSTRUCTIONS>((*instructions)[*index]) == CAL)
                            runtime.emplace(ret_value, VALUE);

                        memory.resize(stack_frame.back());
                        stack_frame.pop_back();
                    } catch(const std::exception & e) {
                        throw err::common((*locations)[*index].line, (*locations)[*index].column, e.what());
                    }
                }
            break;

            case PRC:
                call_info.push(0);
            break;

            case PU:
                memory.push_back(runtime.top());
                runtime.pop();

                ++call_info.top();
            break;

            case RET: {
                if(*program_call_type == CAL) {
                    if(runtime.size() == *program_stack_size)
                        runtime.emplace(var{var::nihil{}}, VALUE);
                    else if(runtime.top().type == REFERENCE) {
                        var ne = get(runtime.top());

                        runtime.pop();
                        runtime.emplace(ne, VALUE);
                    }
                } else if(*program_call_type == CAN && runtime.size() != *program_stack_size)
                    runtime.pop();

                memory.resize(stack_frame.back());
                stack_frame.pop_back();

                program.pop();

                if(program.size() == stopping_point) {
                    var ret = get(runtime.top());
                    runtime.pop();

                    return ret;
                }

                auto & top = program.top();

                index              = &std::get<0>(top);
                instructions       = &std::get<1>(top)->second;
                locations          = &std::get<1>(top)->first;
                program_call_type  = &std::get<2>(top);
                program_stack_size = &std::get<3>(top);
            } break;

            case LOC:
                runtime.emplace(mem_loc(*index), CONSTANT);
            break;

            case LOF:
                if(memory.size() <= mem_loc(*index) + stack_frame.back())
                    memory.resize(mem_loc(*index) + stack_frame.back() + 1);

                runtime.emplace(mem_loc(*index) + stack_frame.back(), REFERENCE);
            break;

            case LOV:
                if(mem_loc(*index) >= stack_frame.size()) throw err::common((*locations)[*index].line, (*locations)[*index].column, "Attempt to access expired variable.");

                if(memory.size() <= mem_loc(*index - 1) + stack_frame[mem_loc(*index)])
                    memory.resize(mem_loc(*index - 1) + stack_frame[mem_loc(*index)] + 1);

                runtime.emplace(mem_loc(*index - 1) + stack_frame[mem_loc(*index)], REFERENCE);
            break;

            case LOE:
                runtime.emplace(mem_loc(*index), EXPOSED);
            break;

            case POP:
                for(std::size_t nillify = memory.size() - mem_loc(*index); nillify < memory.size(); ++nillify)
                    memory[nillify] = vm::holder{};
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
                if(runtime.empty()) throw err::common((*locations)[*index].line, (*locations)[*index].column, "Malformed statement.");

                if(!(bool)(get(runtime.top())))
                    *index = *index + mem_loc(*index) - 1;

                runtime.pop();
            break;

            case JMP:
                *index = *index + mem_loc(*index) - 1;
            break;

            case RJMP:
                *index = *index - mem_loc(*index);
            break;

            case RJE:
                if(runtime.empty()) throw err::common((*locations)[*index].line, (*locations)[*index].column, "Malformed statement.");

                if((bool)(get(runtime.top())))
                    *index = *index - mem_loc(*index);

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

            case GR: binary_op(>);  break;
            case GE: binary_op(>=); break;
            case LR: binary_op(<);  break;
            case LE: binary_op(<=); break;

            case MOV: {
                switch(runtime.top().type) {
                    case VALUE: {
                        const var rhs = get(runtime.top());
                        runtime.pop();

                        switch(runtime.top().type) {
                            case VALUE:
                                throw err::common((*locations)[*index].line, (*locations)[*index].column, "Attempt to initialize a temporary value.");
                            break;

                            case CONSTANT:
                                throw err::common((*locations)[*index].line, (*locations)[*index].column, "Attempt to initialize a constant value");
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
                                throw err::common((*locations)[*index].line, (*locations)[*index].column, "Attempt to initialize a temporary value.");
                            break;

                            case CONSTANT:
                                throw err::common((*locations)[*index].line, (*locations)[*index].column, "Attempt to initialize a constant value");
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
        *index = *index + 1;
    }

    return var::integer{0};
}

#undef mem_loc
#undef binary_op
