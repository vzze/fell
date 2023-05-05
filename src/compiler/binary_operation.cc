#include <compiler.hh>

void fell::compiler::binary_operation(
    const vm::INSTRUCTIONS ins,
    bool & alternance,
    const scan::scanned & data,
    std::stack<vm::INSTRUCTIONS> & operators,
    std::size_t & i,
    instructions & instructions
) {
    using enum fell::scan::TOKENS;

    if(alternance == false) throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected operator.");
    alternance = false;

    switch(data.tokens[i + 1]) {
        case IDENTIFIER:
        case FALSE:
        case TRUE:
        case NIHIL:
        case STRING:
        case NUMBER:
        case INTEGER:
        case LEFT_PAREN:
        case LEFT_CURLY:
        break; // This check should only apply to binary operators

        case BAR:
            if(ins != vm::INSTRUCTIONS::MOV)
                throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected function definition.");
        break;

        default:
            if((data.tokens[i - 1] != IDENTIFIER && data.tokens[i + 1] != RIGHT_PAREN) || data.tokens[i + 1] == SEMICOLON)
                throw err::common(data.locations[i].line, data.locations[i].column, "Expected identifier.");
        break;
    }

    while(!operators.empty() && operator_precedence(operators.top()) >= operator_precedence(ins)) {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(operators.top()));
        operators.pop();
    }

    if(ins != vm::INSTRUCTIONS::PU)
        operators.push(ins);
    else {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(ins));
    }
}
