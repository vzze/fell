#ifndef FELL_HH
#define FELL_HH

#include <compiler.hh>
#include <variable.hh>
#include <scanner.hh>
#include <error.hh>
#include <std.hh>
#include <vm.hh>

#ifdef FELL_DEBUG
#include <debug.hh>
#endif

namespace fell {
    struct interpreter {
        private:
            vm virtual_machine;
            var return_value = var::integer{0};
        public:
            enum class STD_MODULES : std::int32_t {
                ALL,
                GENERAL,
                MATH
            };
        private:
            void init_std_modules(std::vector<STD_MODULES>);
            void init_std_general();
            void init_std_math();
        public:
            interpreter(const std::filesystem::path, std::vector<STD_MODULES> = { STD_MODULES::ALL });

            var & get_exposed(const var::string);
            void register_function(const var::string, std::function<var(lib::params)>);

            var call_exposed_function(const var::string, std::vector<var*>);
            var call_function(var &, std::vector<var*>);

            int cpp_return();
            var main_return();
    };
}

#endif
