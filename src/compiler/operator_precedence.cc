#include <compiler.hh>

std::int32_t fell::compiler::operator_precedence(const vm::INSTRUCTIONS ins) {
    using enum vm::INSTRUCTIONS;

    switch(ins) {
        case LOE:
        case LOV:
        case LOC:
        case LOF:
            return 12;
        break;

        case MAC:
            return 11;
        break;

        case CAL:
        case CAN:
            return 10;
        break;

        case MUL:
        case MOD:
        case DIV:
            return 9;
        break;

        case SUB:
        case ADD:
            return 8;
        break;

        case GR:
        case GE:
        case LR:
        case LE:
            return 7;
        break;

        case NE:
        case EQ:
            return 6;
        break;

        case AN:
            return 4;
        break;

        case OR:
            return 3;
        break;

        case MOV:
            return 2;
        break;

        case PU:
            return 1;
        break;

        default:
            return 0;
        break;
    }
}
