#include <fell.hh>

fell::var & fell::interpreter::get_exposed(const var::string name) {
    return exposed[name];
}

void fell::interpreter::register_function(const var::string name, std::function<var(lib::params)> func) {
    exposed[name] = var{var::func{func}};
}

fell::var fell::interpreter::call_exposed_function(const var::string name, std::vector<var*> params) {
    return virtual_machine.call(exposed[name], params);
}

fell::var fell::interpreter::call_function(var & vr, std::vector<var*> params) {
    return virtual_machine.call(vr, params);
}

void fell::interpreter::init_std_general() {
    auto module = lib::general();

    for(const auto & [name, func] : module)
        register_function(name, func);
}

void fell::interpreter::init_std_math() {
    get_exposed("math") = var::object{lib::math(), {}};
}

void fell::interpreter::init_std_modules(std::vector<STD_MODULES> modules) {
    using enum STD_MODULES;

    for(const auto module : modules) {
        switch(module) {
            case ALL:
                init_std_general();
                init_std_math();
                return;
            break;

            case GENERAL: init_std_general(); break;

            case MATH: init_std_math(); break;
        }
    }
}

fell::interpreter::interpreter(const std::filesystem::path path, std::vector<STD_MODULES> modules) {
    if(path == "version") {
        std::cout << "Fell Copyright (C) 2023 vzze\n";
        std::cout << "This program comes with ABSOLUTELY NO WARRANTY.\n";
        std::cout << "This is free software, and you are welcome to redistribute it\n";
        std::cout << "under certain conditions; type `fell conditions` for details.\n\n";
        std::cout << "Version 0.3.5\n";

        return;
    } else if(path == "conditions") {
        std::cout << "The Fell Programming Language\n";
        std::cout << "Copyright (C) 2023 vzze\n\n";

        std::cout << "This program is free software: you can redistribute it and/or modify\n";
        std::cout << "it under the terms of the GNU General Public License as published by\n";
        std::cout << "the Free Software Foundation, either version 3 of the License, or\n";
        std::cout << "(at your option) any later version.\n\n";

        std::cout << "This program is distributed in the hope that it will be useful,\n";
        std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
        std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
        std::cout << "GNU General Public License for more details.\n\n";

        std::cout << "You should have received a copy of the GNU General Public License\n";
        std::cout << "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";

        return;
    }

    init_std_modules(modules);

#ifdef FELL_DEBUG
    try {
        virtual_machine.cwd = path.parent_path();
        {
            scan::scanned data;

            try {
                data = scan::file(path);
            } catch(const std::exception & e) {
                fell::err::log(e);
                return_value = static_cast<var::integer>(var::string{e.what()}.length());
                return;
            }

            fell::debug::scanner(data);
            fell::compiler::process(data, virtual_machine);
            fell::debug::compiler(virtual_machine);
        }

        return_value = virtual_machine.main();
        fell::debug::vm_memory(virtual_machine);
    } catch(const err::common & e) {
        fell::debug::vm_memory(virtual_machine);
        fell::err::log(e, path.stem().string());

        return_value = static_cast<var::integer>(var::string{e.what()}.length());
    }
#else
    try {
        virtual_machine.cwd = path.parent_path();
        {
            scan::scanned data;

            try {
                data = scan::file(path);
            } catch(const std::exception & e) {
                fell::err::log(e);
                return_value = static_cast<var::integer>(var::string{e.what()}.length());
                return;
            }

            fell::compiler::process(data, virtual_machine);
        }

        return_value = virtual_machine.main();
    } catch(const err::common & e) {
        fell::err::log(e, path.stem().string());
        return_value = static_cast<var::integer>(var::string{e.what()}.length());
    }
#endif
}

fell::var fell::interpreter::main_return() {
    return return_value;
}

int fell::interpreter::cpp_return() {
    using enum var::TYPE;
    switch(return_value.get_type()) {
        case INTEGER:
            return static_cast<int>(return_value.get<var::integer>());
        break;

        case NUMBER:
            return static_cast<int>(return_value.get<var::number>());
        break;

        case NIHIL:  [[fallthrough]];
        case STRING: [[fallthrough]];
        case OBJECT: [[fallthrough]];
        case FUNCTION:
            return 0;
        break;
    }

    return 1602;
}
