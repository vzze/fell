#include <debug.hh>

void fell::debug::vm_memory(vm & vm) {
    using enum fell::var::TYPE;

    std::clog << "\nMEMORY DUMP:\n\n";

    for(auto & v : vm.memory) {
        const auto var = vm.get(v);

        switch(var.get_type()) {
            case INTEGER:
                std::clog << &v << ": INT " << var.get<var::integer>() << '\n';
            break;

            case NUMBER:
                std::clog << &v << ": NUM " << var.get<var::number>() << '\n';
            break;

            case STRING:
                std::clog << &v << ": STR " << std::quoted(var.get<var::string>()) << '\n';
            break;

            case OBJECT:
                std::clog << &v << ": OBJ " << '\n';
            break;

            case NIHIL:
                std::clog << &v << ": NIL " << '\n';
            break;

            case FUNCTION:
                std::clog << &v << ": FUN ";

                if(std::holds_alternative<std::size_t>(var.get<var::func>())) {
                    std::clog << std::get<std::size_t>(var.get<var::func>()) << '\n';
                } else {
                    std::clog << '\n';
                }
            break;
        }
    }

    std::clog << "\nEXPOSED VARIABLES:\n\n";

    for(const auto & [name, var] : exposed) {
        switch(var.get_type()) {
            case INTEGER:
                std::clog << &var << " " << name << ": INT " << var.get<var::integer>() << '\n';
            break;

            case NUMBER:
                std::clog << &var << " " << name << ": NUM " << var.get<var::number>() << '\n';
            break;

            case STRING:
                std::clog << &var << " " << name << ": STR " << std::quoted(var.get<var::string>()) << '\n';
            break;

            case OBJECT:
                std::clog << &var << " " << name << ": OBJ " << '\n';
                for(const auto & [n, v] : var.get<var::object>().first) {
                    switch(v.get_type()) {
                        case INTEGER:
                            std::clog << "    { " << std::quoted(n) << ", " << v.get<var::integer>() << " }" << '\n';
                        break;

                        case NUMBER:
                            std::clog << "    { " << std::quoted(n) << ", " << v.get<var::number>() << " }" << '\n';
                        break;

                        case STRING:
                            std::clog << "    { " << std::quoted(n) << ", " << std::quoted(v.get<var::string>()) << " }" << '\n';
                        break;

                        case OBJECT:
                            std::clog << "    { " << std::quoted(n) << ", " << "OBJ" << " }" << '\n';
                        break;

                        case NIHIL:
                            std::clog << "    { " << std::quoted(n) << ", " << "NIL" << " }" << '\n';
                        break;

                        case FUNCTION:
                            std::clog << "    { " << std::quoted(n) << ", " << "FUN" << " }" << '\n';
                        break;
                    }
                }

                for(const auto & v : var.get<var::object>().second) {
                    switch(v.get_type()) {
                        case INTEGER:
                            std::clog << "    { " << v.get<var::integer>() << " }" << '\n';
                        break;

                        case NUMBER:
                            std::clog << "    { " << v.get<var::number>() << " }" << '\n';
                        break;

                        case STRING:
                            std::clog << "    { " << std::quoted(v.get<var::string>()) << " }" << '\n';
                        break;

                        case OBJECT:
                            std::clog << "    { " << "OBJ" << " }" << '\n';
                        break;

                        case NIHIL:
                            std::clog << "    { " << "NIL" << " }" << '\n';
                        break;

                        case FUNCTION:
                            std::clog << "    { " << "FUN" << " }" << '\n';
                        break;
                    }
                }
            break;

            case NIHIL:
                std::clog << &var << " " << name << ": NIL " << '\n';
            break;

            case FUNCTION:
                std::clog << &var << " " << name << ": FUN " << '\n';
            break;
        }
    }

}
