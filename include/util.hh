#ifndef UTIL_HH
#define UTIL_HH

#include <type_traits>
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <string>
#include <stack>

#include "variable.hh"

namespace fell {
    namespace util {
        void copy(types::variable::var &, const types::variable::var &);
        std::string get_file(const std::filesystem::path);

        void trim(std::string &);
        void remove_comments(std::string &);
        void check_paren(const std::string &);

        template<typename T, typename ... Args>
        inline types::variable::var make_var(Args && ... args) {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        inline T & get_value(const types::variable::var & var) {
            try {
                return std::any_cast<T&>(var->value);
            } catch(...) { // Need to change bad_any_cast error into something more user friendly
                if constexpr(std::is_same_v<T, types::number::num>) {
                    throw std::runtime_error("Right hand variable is not convertible to Number.");
                } else if constexpr(std::is_same_v<T, types::string::str>) {
                    throw std::runtime_error("Right hand variable is not convertible to String.");
                } else if constexpr(std::is_same_v<T, types::table::tbl>) {
                    throw std::runtime_error("Right hand variable is not convertible to Table.");
                } else if constexpr(std::is_same_v<T, types::nihil::nil>) {
                    throw std::runtime_error("Right hand variable is not convertible to Nil.");
                }
            }
        }

        template<typename T>
        inline T & get_value(types::variable * var) {
            try {
                return std::any_cast<T&>(var->value);
            } catch(...) { // Same thing here as above
                if constexpr(std::is_same_v<T, types::number::num>) {
                    throw std::runtime_error("Right hand variable is not convertible to Number.");
                } else if constexpr(std::is_same_v<T, types::string::str>) {
                    throw std::runtime_error("Right hand variable is not convertible to String.");
                } else if constexpr(std::is_same_v<T, types::table::tbl>) {
                    throw std::runtime_error("Right hand variable is not convertible to Table.");
                } else if constexpr(std::is_same_v<T, types::nihil::nil>) {
                    throw std::runtime_error("Right hand variable is not convertible to Nil.");
                }
            }
        }
    }
}

#endif
