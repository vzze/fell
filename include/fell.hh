#ifndef FELL_HH
#define FELL_HH

#include <compiler.hh>
#include <variable.hh>
#include <scanner.hh>
#include <error.hh>
#include <std.hh>
#include <vm.hh>

#ifdef DEBUG
#include <debug.hh>
#endif

namespace fell {
    struct interpreter {
        private:
            vm vm;
            var return_value = var::integer{0};
        public:
            interpreter(const std::filesystem::path);
            var & get_exposed(const var::string);
            void register_function(const var::string, std::function<var(lib::params)>);

            var call_exposed_function(const var::string, std::vector<var*>);
            var call_function(var &, std::vector<var*>);

            int cpp_return();
            var main_return();
    };
}

#endif
