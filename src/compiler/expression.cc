#include <compiler.hh>
#include <error.hh>

void fell::compiler::expression(
    const scan::scanned & data,
    std::size_t & i,
    std::size_t & identifier_count,
    std::size_t & f_count,
    std::size_t & i_count,
    std::size_t & s_count,
    std::list<
        std::pair<
            std::list<std::unordered_map<std::string, std::int32_t>>,
            std::vector<bool>
        >
    > & contexts,
    context_solver & ctx_solver,
    instructions & instructions,
    scan::TOKENS end_token
) {
    using enum scan::TOKENS;

    std::stack<vm::INSTRUCTIONS> operators;
    std::stack<bool> func_call;
    std::stack<bool> void_call;

    std::size_t paren_counter = 0;

    bool alternance = false;

    while(data.tokens[i] != end_token) {
        switch(data.tokens[i]) {
            case LEFT_CURLY:
                if(alternance == true) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Unexpected Object definition."
                );

                alternance = true;

                if(data.tokens[i + 1] == RIGHT_CURLY) {
                    instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                    constants.emplace_back(var::object{});
                    operators.push(vm::INSTRUCTIONS::LOC);
                    ++i;
                } else {
                    throw err::common(
                        data.locations[i].line,
                        data.locations[i].column,
                        "Object type does not support an initializer list, yet."
                    );
                }
            break;

            case END:
                throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Unterminated statement."
                );
            break;

            case BAR: {
                ctx_solver.contexts.push(CONTEXT::FUNCTION);
                ++ctx_solver.in_func;

                ++i;

                contexts.emplace_back();
                contexts.rbegin()->first.emplace_back();

                std::int32_t x = 0;

                while(data.tokens[i] == IDENTIFIER) {
                    (*contexts.rbegin()->first.begin())[data.identifiers[identifier_count]] = x;

                    ++identifier_count;
                    ++x;
                    ++i;
                }

                if(contexts.rbegin()->first.begin()->size() != static_cast<std::size_t>(x)) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Function parameters can't have the same name."
                );

                contexts.rbegin()->second.resize(static_cast<std::size_t>(x), true);

                if(data.tokens[i] != BAR) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Malformed function definition."
                );

                ++i;

                if(data.tokens[i] != LEFT_CURLY) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Malformed function definition."
                );

                instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::LOC));
                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::MOV));

                instructions.ins.push(constants.size());

                labels.emplace_back();
                constants.emplace_back(var::func{labels.size() - 1});

                return;

            } break;

            case IDENTIFIER: {
                if(alternance == true) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Unexpected identifier: " + data.identifiers[identifier_count]
                );

                alternance = true;

                if(auto global = exposed.find(data.identifiers[identifier_count]); global != exposed.end()) {
                    auto it = std::find_if(constants.begin(), constants.end(), [&](const var & el) -> bool {
                        if(el.get_type() == var::TYPE::STRING) {
                            if(el.get<var::string>() == global->first)
                                return true;
                            else
                                return false;
                        } else {
                            return false;
                        }
                    });

                    if(it != constants.end()) {
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(it - constants.begin()));
                    } else {
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                        constants.emplace_back(global->first);
                    }

                    operators.push(vm::INSTRUCTIONS::LOE);

                    ++identifier_count;
                } else {
                    std::unordered_map<std::string, std::int32_t>::iterator it;
                    std::int32_t stack_frame = static_cast<std::int32_t>(contexts.size());

                    bool found_var = false;

                    for(auto context = contexts.rbegin(); context != contexts.rend(); ++context) {
                        --stack_frame;

                        for(auto ctx = context->first.rbegin(); ctx != context->first.rend(); ++ctx) {
                            it = ctx->find(data.identifiers[identifier_count]);
                            if(it != ctx->end()) {
                                found_var = true;
                                goto found;
                            }
                        }
                    }

                    stack_frame = static_cast<std::int32_t>(contexts.size()) - 1;
found:
                    if(found_var) {
                        instructions.top(true, i)->push_back(it->second);
                        if(stack_frame == static_cast<std::int32_t>(contexts.size()) - 1)
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::TOP));
                        else
                            instructions.top(true, i)->push_back(stack_frame);
                    } else {
                        bool found = false;
                        std::vector<bool> & memory_slots = contexts.rbegin()->second;

                        for(std::size_t i = 0; i < memory_slots.size(); ++i) {
                            if(memory_slots[i] == false) {
                                found = true;

                                (*contexts.rbegin()->first.rbegin())[
                                    data.identifiers[identifier_count]
                                ] = static_cast<std::int32_t>(i);

                                instructions.top(true, i)->push_back(static_cast<std::int32_t>(i));
                                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::TOP));

                                memory_slots[i] = true;
                                break;
                            }
                        }

                        if(!found) {
                            memory_slots.push_back(true);
                            (*contexts.rbegin()->first.rbegin())[
                                data.identifiers[identifier_count]
                            ] = static_cast<std::int32_t>(memory_slots.size()) - 1;

                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(memory_slots.size()) - 1);
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::TOP));
                        }
                    }

                    operators.push(vm::INSTRUCTIONS::LOV);

                    ++identifier_count;
                }
            } break;

            case NUMBER:  constant_float(data, i, f_count, alternance, instructions, operators);   break;
            case INTEGER: constant_integer(data, i, i_count, alternance, instructions, operators); break;
            case STRING:  constant_string(data, i, s_count, alternance, instructions, operators);  break;

            case FALSE:
                instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                constants.emplace_back(var::integer{0});
                operators.push(vm::INSTRUCTIONS::LOC);
            break;

            case TRUE:
                instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                constants.emplace_back(var::integer{1});
                operators.push(vm::INSTRUCTIONS::LOC);
            break;

            case NIHIL:
                instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
                constants.emplace_back(var::nihil{0});
                operators.push(vm::INSTRUCTIONS::LOC);
            break;

            case LEFT_SQUARE:
                binary_operation(vm::INSTRUCTIONS::MAC, alternance, data, operators, i, instructions);
                operators.push(vm::INSTRUCTIONS::PAR);
            break;

            case RIGHT_SQUARE:
                while(!operators.empty() && operators.top() != vm::INSTRUCTIONS::PAR) {
                    instructions.top(true, i)->push_back(static_cast<std::int32_t>(operators.top()));
                    operators.pop();
                }

                if(operators.empty()) throw err::common(data.locations[i].line, data.locations[i].column, "Missmatched square brackets.");

                operators.pop();
            break;

            case LEFT_PAREN:
                ++paren_counter;
                if(alternance == true) {
                    func_call.push(true);

                    if(data.tokens[i + 1] == RIGHT_PAREN) void_call.push(true);
                    else void_call.push(false);

                    if(i <= 1 || (i > 1 && (data.tokens[i - 2] == SEMICOLON || data.tokens[i - 2] == LEFT_CURLY)))
                        binary_operation(vm::INSTRUCTIONS::CAN, alternance, data, operators, i, instructions);
                    else
                        binary_operation(vm::INSTRUCTIONS::CAL, alternance, data, operators, i, instructions);

                    instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::PRC));
                }

                operators.push(vm::INSTRUCTIONS::PAR);
            break;

            case RIGHT_PAREN:
                --paren_counter;

                while(!operators.empty() && operators.top() != vm::INSTRUCTIONS::PAR) {
                    instructions.top(true, i)->push_back(static_cast<std::int32_t>(operators.top()));
                    operators.pop();
                }

                if(operators.empty()) throw err::common(data.locations[i].line, data.locations[i].column, "Missmatched parenthesis.");

                operators.pop();

                if(!func_call.empty()) {
                    alternance = true;

                    if(void_call.top()) {
                        void_call.pop();
                    } else {
                        void_call.pop();
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::PU));
                    }

                    func_call.pop();
                }
            break;

            case COMMA:
                if(func_call.empty()) throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected comma.");

                binary_operation(vm::INSTRUCTIONS::PU, alternance, data, operators, i, instructions);
            break;

            case STAR:          binary_operation(vm::INSTRUCTIONS::MUL, alternance, data, operators, i, instructions); break;
            case MODULO:        binary_operation(vm::INSTRUCTIONS::MOD, alternance, data, operators, i, instructions); break;
            case SLASH:         binary_operation(vm::INSTRUCTIONS::DIV, alternance, data, operators, i, instructions); break;

            case PLUS:          binary_operation(vm::INSTRUCTIONS::ADD, alternance, data, operators, i, instructions); break;
            case MINUS:         binary_operation(vm::INSTRUCTIONS::SUB, alternance, data, operators, i, instructions); break;

            case GREATER:       binary_operation(vm::INSTRUCTIONS::GR, alternance, data, operators, i, instructions);  break;
            case GREATER_EQUAL: binary_operation(vm::INSTRUCTIONS::GE, alternance, data, operators, i, instructions);  break;
            case LESS:          binary_operation(vm::INSTRUCTIONS::LR, alternance, data, operators, i, instructions);  break;
            case LESS_EQUAL:    binary_operation(vm::INSTRUCTIONS::LE, alternance, data, operators, i, instructions);  break;

            case BANG_EQUAL:    binary_operation(vm::INSTRUCTIONS::NE, alternance, data, operators, i, instructions);  break;
            case EQUAL_EQUAL:   binary_operation(vm::INSTRUCTIONS::EQ, alternance, data, operators, i, instructions);  break;

            case AND:           binary_operation(vm::INSTRUCTIONS::AN, alternance, data, operators, i, instructions);  break;

            case OR:            binary_operation(vm::INSTRUCTIONS::OR, alternance, data, operators, i, instructions);  break;

            case EQUAL:         binary_operation(vm::INSTRUCTIONS::MOV, alternance, data, operators, i, instructions); break;

            case DOT:           binary_operation(vm::INSTRUCTIONS::MAC, alternance, data, operators, i, instructions); break;

            default:
                throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected expression.");
            break;
        }

        ++i;
    }

    if(paren_counter) throw err::common(data.locations[i].line, data.locations[i].column, "Missmatched parenthesis.");

    while(!operators.empty()) {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(operators.top()));
        operators.pop();
    }

    while(data.tokens[i] == SEMICOLON) ++i;
    --i;
}
