#ifndef OVERRIDE_HH
#define OVERRIDE_HH

#include <filesystem>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <string>

#include "variable.hh"

namespace fell {
    namespace util {
        void override(types::variable::var &, const types::variable::var &);
        std::string get_file(const std::filesystem::path);

        template<typename T, typename ... Args>
        inline types::variable::var make_var(Args && ... args) {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        inline T & get_value(const types::variable::var & var) {
            return std::any_cast<T&>(var->value);
        }

        template<typename T>
        inline T & get_value(types::variable * var) {
            return std::any_cast<T&>(var->value);
        }
    }
}

#endif
