#ifndef STD_HH
#define STD_HH

#include <functional>
#include <numeric>
#include <numbers>
#include <random>
#include <cmath>

#include <variable.hh>
#include <vm.hh>

namespace fell::lib {
    struct params {
        private:
            std::size_t offset;
            std::size_t size;
            vm * ref;
        public:
            vm::stack & get_stack();
            void pop_stack_frame();

            std::filesystem::path & cwd();

            var & operator [] (const std::size_t);
            void for_each(std::function<void(var &)>, const std::size_t = 0);

            var call_function(var &, std::vector<var*>);
            var call_module(std::pair<std::vector<scan::location>, std::vector<std::int32_t>> &);

            std::size_t number() const;
            params(struct vm *, const std::size_t, const std::size_t);
    };

    std::vector<std::pair<var::string, std::function<var(params)>>> general();
    std::unordered_map<var::string, var> math();
};

#endif
