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
    program.push({0, &main_program, vm::INSTRUCTIONS::CAL, 0});

    stack_frame.push_back(0);
    current_stack_frame = 0;
}

fell::var fell::vm::call(fell::var & vr, std::vector<var*> params) {
    stack_frame.push_back(memory.size());
    current_stack_frame = stack_frame.back();

    for(var * param : params)
        memory.emplace_back(param);

    program.push({0, &labels[std::get<std::size_t>(vr.get<var::func>())], vm::INSTRUCTIONS::CAL, runtime.size()});

    return run(program.size() - 1);
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
            [[likely]] case CAL:
                current_stack_frame = stack_frame.back();

                if(get(runtime.top()).get_type() != var::TYPE::FUNCTION)
                    throw err::common((*locations)[(*index)].line, (*locations)[*index].column,  "Attempt to call a non-function variable.");

                if(std::holds_alternative<std::size_t>(get(runtime.top()).get<var::func>())) {
                    auto & value = labels[std::get<std::size_t>(get(runtime.top()).get<var::func>())];
                    runtime.pop();

                    program.push({0, &value, static_cast<INSTRUCTIONS>((*instructions)[*index]), runtime.size()});

                    index              = &std::get<0>(program.top());
                    instructions       = &std::get<1>(program.top())->second;
                    locations          = &std::get<1>(program.top())->first;
                    program_call_type  = &std::get<2>(program.top());
                    program_stack_size = &std::get<3>(program.top());
                    continue;
                } else {
                    try {
                        const auto & fn = std::get<std::function<fell::var(lib::params)>>(get(runtime.top()).get<var::func>());
                        runtime.pop();

                        var ret_value = fn({
                            this,
                            current_stack_frame, memory.size() - current_stack_frame
                        });

                        if(static_cast<INSTRUCTIONS>((*instructions)[*index]) == CAL)
                            runtime.emplace(ret_value, VALUE);

                        memory.resize(stack_frame.back());
                        stack_frame.pop_back();

                        current_stack_frame = stack_frame.back();
                    } catch(const std::exception & e) {
                        throw err::common((*locations)[*index].line, (*locations)[*index].column, e.what());
                    }
                }
            break;

            case PRC:
                stack_frame.push_back(memory.size());
            break;

            case PU:
                memory.push_back(runtime.top());
                runtime.pop();
            break;

            case RET:
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

                memory.resize(current_stack_frame);
                stack_frame.pop_back();

                program.pop();

                if(program.size() != 0) [[likely]]
                    current_stack_frame = stack_frame.back();

                if(program.size() == stopping_point) {
                    var ret = get(runtime.top());
                    runtime.pop();

                    return ret;
                }

                index              = &std::get<0>(program.top());
                instructions       = &std::get<1>(program.top())->second;
                locations          = &std::get<1>(program.top())->first;
                program_call_type  = &std::get<2>(program.top());
                program_stack_size = &std::get<3>(program.top());
            break;

            case LOC:
                runtime.emplace(mem_loc(*index), CONSTANT);
            break;

            case LOF:
                if(memory.size() <= mem_loc(*index) + current_stack_frame)
                    memory.resize(mem_loc(*index) + current_stack_frame + 1);

                runtime.emplace(mem_loc(*index) + current_stack_frame, REFERENCE);
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
