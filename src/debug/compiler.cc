#include <debug.hh>
#include <error.hh>

void fell::debug::compiler(const vm & vm) {
    using enum vm::INSTRUCTIONS;

    std::clog << "\n\nBYTECODE:\n\n";

    const auto display = [&](const auto count) {
        std::clog << '\n' << std::setfill('0') << std::setw(5) << std::hex << count << std::dec << ": " ;
    };

    const auto output_instruction = [&](const auto i, const auto instructions) {
        switch(static_cast<vm::INSTRUCTIONS>(instructions[i])) {
            case LOE:
                display(i);
                std::clog << "LOE  " << instructions[i - 1];
            break;

            case LOV:
                display(i);
                switch(static_cast<vm::INSTRUCTIONS>(instructions[i - 1])) {
                    case TOP:
                        std::clog << "LOV  TOP " << instructions[i - 2];
                    break;

                    default:
                        std::clog << "LOV  " << instructions[i - 1] << ' ' << instructions[i - 2];
                    break;
                }
            break;

            case LOC:
                display(i);
                std::clog << "LOC  " << instructions[i - 1];
            break;

            case JMP:
                display(i);
                std::clog << "JMP  " << instructions[i - 1];
            break;

            case RJMP:
                display(i);
                std::clog << "RJMP " << instructions[i - 1];
            break;

            case JE:
                display(i);
                std::clog << "JE   " << instructions[i - 1];
            break;

            case RJE:
                display(i);
                std::clog << "RJE  " << instructions[i - 1];
            break;

            case POP:
                display(i);
                std::clog << "POP  " << instructions[i - 1];
            break;

            case PU:
                display(i);
                std::clog << "PU   ";
            break;

            case PRC:
                display(i);
                std::clog << "PRC  ";
            break;

            case CAL:
                display(i);
                std::clog << "CAL  ";
            break;

            case CAN:
                display(i);
                std::clog << "CAN  ";
            break;

            case MAC:
                display(i);
                std::clog << "MAC  ";
            break;

            case MOV:
                display(i);
                std::clog << "MOV  ";
            break;

            case MUL:
                display(i);
                std::clog << "MUL  ";
            break;

            case DIV:
                display(i);
                std::clog << "DIV  ";
            break;

            case MOD:
                display(i);
                std::clog << "MOD  ";
            break;

            case SUB:
                display(i);
                std::clog << "SUB  ";
            break;

            case ADD:
                display(i);
                std::clog << "ADD  ";
            break;

            case AN:
                display(i);
                std::clog << "AN   ";
            break;

            case OR:
                display(i);
                std::clog << "OR   ";
            break;

            case LE:
                display(i);
                std::clog << "LE   ";
            break;

            case LR:
                display(i);
                std::clog << "LR   ";
            break;

            case GE:
                display(i);
                std::clog << "GE   ";
            break;

            case GR:
                display(i);
                std::clog << "GR   ";
            break;

            case EQ:
                display(i);
                std::clog << "EQ   ";
            break;

            case NE:
                display(i);
                std::clog << "NE   ";
            break;

            case RET:
                display(i);
                std::clog << "RET  ";
            break;

            default: break;
        }
    };

    for(std::size_t i = 0; const auto & l : labels) {
        std::clog << "LABEL " << i << ":\n";

        for(std::size_t j = 0; j < l.second.size(); ++j)
            output_instruction(j, l.second);

        std::clog << "\n\n";

        ++i;
    }

    std::clog << "MAIN:\n";

    for(std::size_t i = 0; i < vm.ins.second.size(); ++i) {
        output_instruction(i, vm.ins.second);
    }

    std::clog << "\n\n";
}
