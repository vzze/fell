#include <compiler.hh>
#include <error.hh>

std::vector<std::int32_t> * fell::compiler::instructions::top([[maybe_unused]] const bool push, [[maybe_unused]] const std::size_t i) {
    if(ins.empty()) {
        if(push) vm->main_program.first.push_back(data->locations[i]);

        return &vm->main_program.second;
    } else {
        if(push) labels[std::get<std::size_t>(constants[ins.top()].get<var::func>())].first.push_back(data->locations[i]);

        return &labels[std::get<std::size_t>(constants[ins.top()].get<var::func>())].second;
    }
}

void fell::compiler::instructions::pop() { return ins.pop(); }

#define ERR(arg) \
    throw err::common(data.locations[i].line, data.locations[i].column, arg) \

void fell::compiler::process(const scan::scanned & data, fell::vm & vm) {
    using enum scan::TOKENS;

    std::size_t identifier_count{0};
    std::size_t const_f_count{0};
    std::size_t const_i_count{0};
    std::size_t const_s_count{0};

    std::list<
        std::pair<
            std::list<std::unordered_map<std::string, std::int32_t>>,
            std::vector<bool>
        >
    > contexts = { { { {} }, {} } };

    context_solver ctx_solver;

    instructions instructions{{}, &data, &vm};

    if(*data.tokens.begin() == END)
        throw err::common(1, 1, "Empty file.");

    for(std::size_t i = 0; i < data.tokens.size(); ++i) {
        switch(data.tokens[i]) {
            case EXPOSED:
                if(data.tokens[i + 1] != IDENTIFIER) ERR("Malformed exposed variable declaration.");

                if(exposed.end() != exposed.find(data.identifiers[identifier_count]))
                    ERR("Redefinition of an exposed variable " + data.identifiers[identifier_count] + ".");

                exposed[data.identifiers[identifier_count]] = var::nihil{};
            break;

            case LEFT_CURLY:
                contexts.rbegin()->first.emplace_back();
                ctx_solver.contexts.push(CONTEXT::BLOCK);
            break;

            case RIGHT_CURLY: {
                if(contexts.rbegin()->first.size() == 1 && contexts.size() == 1) throw err::common(
                    data.locations[i].line,
                    data.locations[i].column,
                    "Unexpected right curly."
                );

                for(const auto & [name, slot] : *contexts.rbegin()->first.rbegin())
                    contexts.rbegin()->second[static_cast<std::size_t>(slot)] = false;

                switch(ctx_solver.contexts.top()) {
                    case CONTEXT::ELSE_IF: {
                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        auto index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        if(data.tokens[i + 1] != ELSE)
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index - 1);
                        else
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index + 1);

                        index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        if(data.tokens[i + 1] != ELSE)
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index - 1);
                        else
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index);

                        contexts.rbegin()->first.pop_back();
                    } break;

                    case CONTEXT::ELSE: {
                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        const auto index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index - 1);

                        contexts.rbegin()->first.pop_back();
                    } break;

                    case CONTEXT::IF: {
                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        const auto index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        if(data.tokens[i + 1] != ELSE)
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index - 1);
                        else
                            (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index + 1);

                        contexts.rbegin()->first.pop_back();
                    } break;

                    case CONTEXT::WHILE: {
                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        const auto index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        instructions.top(true, i)->push_back(-static_cast<std::int32_t>(index - instructions.top()->size() + static_cast<std::size_t>((*instructions.top())[index - 1])));
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::RJMP));

                        (*instructions.top())[index - 1] = static_cast<std::int32_t>(instructions.top()->size() - index);

                        contexts.rbegin()->first.pop_back();
                    } break;

                    case CONTEXT::DO_WHILE: {
                        const auto index = static_cast<std::size_t>(ctx_solver.jump_table.top());
                        ctx_solver.jump_table.pop();

                        ++i;
                        if(data.tokens[i] != WHILE) ERR("Malformed do-while block.");

                        ++i;
                        if(data.tokens[i] == SEMICOLON) ERR("Empty while in do-while block.");

                        expression(
                            data,
                            i,
                            identifier_count,
                            const_f_count,
                            const_i_count,
                            const_s_count,
                            contexts,
                            ctx_solver,
                            instructions,
                            SEMICOLON
                        );

                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        instructions.top(true, i)->push_back(-static_cast<std::int32_t>(index - instructions.top()->size()) + 1);
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::RJE));

                        contexts.rbegin()->first.pop_back();
                    } break;

                    case CONTEXT::FUNCTION:
                        ++i;

                        if(data.tokens[i] != SEMICOLON) ERR("Missing semicolon.");

                        if(instructions.top()->empty() || static_cast<vm::INSTRUCTIONS>((*instructions.top()->rbegin())) != vm::INSTRUCTIONS::RET)
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::RET));

                        instructions.pop();
                        --ctx_solver.in_func;
                        contexts.pop_back();
                    break;

                    case CONTEXT::BLOCK:
                        if(contexts.rbegin()->first.rbegin()->size() != 0) {
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(contexts.rbegin()->first.rbegin()->size()));
                            instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::POP));
                        }

                        contexts.rbegin()->first.pop_back();
                    break;
                }

                ctx_solver.contexts.pop();

            } break;

            case IDENTIFIER:
                if(data.tokens[i + 1] == SEMICOLON) ERR("Statement has no effect.");
                expression(
                    data,
                    i,
                    identifier_count,
                    const_f_count,
                    const_i_count,
                    const_s_count,
                    contexts,
                    ctx_solver,
                    instructions,
                    SEMICOLON
                );
            break;

            case BAR:           ERR("Unexpected expression."); break;
            case AND:           ERR("Unexpected expression."); break;
            case OR:            ERR("Unexpected expression."); break;
            case INTEGER:       ERR("Unexpected expression."); break;
            case NUMBER:        ERR("Unexpected expression."); break;
            case STRING:        ERR("Unexpected expression."); break;
            case NIHIL:         ERR("Unexpected expression."); break;
            case FALSE:         ERR("Unexpected expression."); break;
            case TRUE:          ERR("Unexpected expression."); break;
            case DOT:           ERR("Unexpected expression."); break;
            case STAR:          ERR("Unexpected expression."); break;
            case SLASH:         ERR("Unexpected expression."); break;
            case MODULO:        ERR("Unexpected expression."); break;
            case PLUS:          ERR("Unexpected expression."); break;
            case MINUS:         ERR("Unexpected expression."); break;
            case COMMA:         ERR("Unexpected expression."); break;
            case SEMICOLON:     ERR("Empty statement."      ); break;
            case BANG:          ERR("Unexpected expression."); break;
            case BANG_EQUAL:    ERR("Unexpected expression."); break;
            case EQUAL:         ERR("Unexpected expression."); break;
            case EQUAL_EQUAL:   ERR("Unexpected expression."); break;
            case GREATER:       ERR("Unexpected expression."); break;
            case GREATER_EQUAL: ERR("Unexpected expression."); break;
            case LESS:          ERR("Unexpected expression."); break;
            case LESS_EQUAL:    ERR("Unexpected expression."); break;
            case LEFT_PAREN:    ERR("Unexpected expression."); break;
            case RIGHT_PAREN:   ERR("Unexpected expression."); break;
            case LEFT_SQUARE:   ERR("Unexpected expression."); break;
            case RIGHT_SQUARE:  ERR("Unexpected expression."); break;

            case IF:
                ++i;
                if(data.tokens[i] == LEFT_CURLY) ERR("Empty if.");

                expression(
                    data,
                    i,
                    identifier_count,
                    const_f_count,
                    const_i_count,
                    const_s_count,
                    contexts,
                    ctx_solver,
                    instructions,
                    LEFT_CURLY
                );

                ++i;
                contexts.rbegin()->first.emplace_back();

                instructions.top(true, i)->push_back(0);
                ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                ctx_solver.contexts.push(CONTEXT::IF);

                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::JE));
            break;

            case ELSE:
                if(!(data.tokens[i + 1] == LEFT_CURLY || data.tokens[i + 1] == IF)) ERR("Malformed else.");

                if(data.tokens[i - 1] == RIGHT_CURLY) {
                    if(data.tokens[i + 1] == IF) {
                        i += 2;
                        if(data.tokens[i] == LEFT_CURLY) ERR("Empty if.");

                        instructions.top(true, i)->push_back(0);
                        ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::JMP));

                        expression(
                            data,
                            i,
                            identifier_count,
                            const_f_count,
                            const_i_count,
                            const_s_count,
                            contexts,
                            ctx_solver,
                            instructions,
                            LEFT_CURLY
                        );

                        ++i;
                        contexts.rbegin()->first.emplace_back();

                        instructions.top(true, i)->push_back(0);
                        ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                        ctx_solver.contexts.push(CONTEXT::ELSE_IF);

                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::JE));
                    } else {
                        instructions.top(true, i)->push_back(0);
                        ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                        ctx_solver.contexts.push(CONTEXT::ELSE);

                        contexts.rbegin()->first.emplace_back();
                        ++i;

                        instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::JMP));
                    }
                } else {
                    ERR("Extraneous else.");
                }
            break;

            case RETURN:
                ++i;

                if(data.tokens[i] != SEMICOLON)
                    expression(
                        data,
                        i,
                        identifier_count,
                        const_f_count,
                        const_i_count,
                        const_s_count,
                        contexts,
                        ctx_solver,
                        instructions,
                        SEMICOLON
                    );

                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::RET));
            break;

            case FOR: ERR("Not implemented yet."); break;

            case WHILE: {
                ++i;
                if(data.tokens[i] == LEFT_CURLY) ERR("Empty while.");

                std::int32_t current_size = static_cast<std::int32_t>(instructions.top()->size());

                expression(
                    data,
                    i,
                    identifier_count,
                    const_f_count,
                    const_i_count,
                    const_s_count,
                    contexts,
                    ctx_solver,
                    instructions,
                    LEFT_CURLY
                );

                ++i;
                contexts.rbegin()->first.emplace_back();

                current_size -= static_cast<std::int32_t>(instructions.top()->size()) + 2;

                instructions.top(true, i)->push_back(current_size);
                ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                ctx_solver.contexts.push(CONTEXT::WHILE);

                instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::JE));
            } break;

            case DO:
                ++i;
                contexts.rbegin()->first.emplace_back();

                if(data.tokens[i] != LEFT_CURLY) ERR("Malformed do-while block.");

                ctx_solver.jump_table.push(static_cast<std::int32_t>(instructions.top()->size()));
                ctx_solver.contexts.push(CONTEXT::DO_WHILE);
            break;

            case END:
                if(contexts.rbegin()->first.size() != 1)
                    throw err::common("Missmatched curly brackets.");

                if(static_cast<vm::INSTRUCTIONS>(*instructions.top()->rbegin()) != vm::INSTRUCTIONS::RET)
                    instructions.top(true, i)->push_back(static_cast<std::int32_t>(vm::INSTRUCTIONS::RET));

                return;
            break;
        }
    }
}

#undef ERR
