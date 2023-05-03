#include <fell.hh>

fell::var & fell::interpreter::get_exposed(const var::string name) {
    return exposed[name];
}

void fell::interpreter::register_function(const var::string name, std::function<var(lib::params)> func) {
    exposed[name] = var{var::func{func}};
}

fell::var fell::interpreter::call_function(const var::string name, std::vector<var*> params) {
    vm.call_info.top().push(params.size());

    vm.runtime.emplace(exposed[name], vm::holder::TYPE::EXPOSED);

    for(var * param : params)
        vm.memory.emplace_back(param);

    vm.call({0, 0}, vm::INSTRUCTIONS::CAL);

    var v = vm.get(vm.runtime.top());
    vm.runtime.pop();

    return v;
}

fell::var fell::interpreter::call_function(var & vr, std::vector<var*> params) {
    vm.call_info.top().push(params.size());

    vm.runtime.emplace(vr, vm::holder::TYPE::VALUE);

    for(var * param : params)
        vm.memory.emplace_back(param);

    vm.call({0, 0}, vm::INSTRUCTIONS::CAL);

    var v = vm.get(vm.runtime.top());
    vm.runtime.pop();

    return v;
}


fell::interpreter::interpreter(const std::filesystem::path path) {
    if(path == "version") {
        std::cout << "Fell Copyright (C) 2023 vzze\n";
        std::cout << "This program comes with ABSOLUTELY NO WARRANTY.\n";
        std::cout << "This is free software, and you are welcome to redistribute it\n";
        std::cout << "under certain conditions; type `fell conditions` for details.\n\n";
        std::cout << "Version 0.2.0\n";

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

    for(const auto & [name, func] : lib::general)
        register_function(name, func);

    try {
        vm.cwd = path.parent_path();
        {
            auto data = scan::file(path);

#ifdef DEBUG
            fell::debug::scanner(data);
#endif
            fell::compiler::process(data, vm);
#ifdef DEBUG
            fell::debug::compiler(vm);
#endif
        }

        vm.run(vm.ins);

#ifdef DEBUG
        fell::debug::vm_memory(vm);
#endif

    } catch(const std::exception & e) {
        fell::err::log(e);
    }
}
