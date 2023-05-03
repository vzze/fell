#ifndef STD_HH
#define STD_HH

#include <functional>

#include <variable.hh>
#include <vm.hh>

namespace fell::lib {
    struct params {
        private:
            std::size_t offset;
            std::size_t size;
            vm * vm;
        public:
            std::stack<fell::vm::holder> & get_stack();
            std::filesystem::path & cwd();

            var & operator [] (const std::size_t);
            void for_each(std::function<void(var &)>, const std::size_t = 0);

            var call_function(var &, std::vector<var*>);

            std::size_t number() const;
            params(struct vm *, const std::size_t, const std::size_t);
    };

    extern std::vector<std::pair<std::string, std::function<var(params)>>> general;
};

#endif
