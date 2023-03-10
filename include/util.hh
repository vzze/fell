#ifndef UTIL_HH
#define UTIL_HH


#include <type_traits>
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "variable.hh"

namespace fell {
    namespace util {
        void copy(types::variable::var &, const types::variable::var &);
        std::string get_file(const std::filesystem::path);

        void remove_comments(std::string &);
        double str_view_tod(std::string_view);

        std::size_t parse_args(int, char**);

        template<typename T, typename ... Args>
        inline types::variable::var make_var(Args && ... args) {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        inline T & get_value(types::variable * var) {
            try {
                return std::any_cast<T&>(var->value);
            } catch(...) { // Get underlying data from a variable
                if constexpr(std::is_same_v<T, types::number::num>) {
                    throw std::runtime_error("Variable is not convertible to Number.");
                } else if constexpr(std::is_same_v<T, types::string::str>) {
                    throw std::runtime_error("Variable is not convertible to String.");
                } else if constexpr(std::is_same_v<T, types::table::tbl>) {
                    throw std::runtime_error("Variable is not convertible to Table.");
                } else if constexpr(std::is_same_v<T, types::nihil::nil>) {
                    throw std::runtime_error("Variable is not convertible to Nil.");
                } else if constexpr(std::is_same_v<T, types::func::data>) {
                    throw std::runtime_error("Variable is not convertible to Function.");
                } else if constexpr(std::is_same_v<T, types::file::fl>) {
                    throw std::runtime_error("Variable is not convertible to File.");
                }
            }
        }

        template<typename T>
        inline const T & get_value(const types::variable * var) {
            try {
                return std::any_cast<const T&>(var->value);
            } catch(...) { // Same thing here as above
                if constexpr(std::is_same_v<T, types::number::num>) {
                    throw std::runtime_error("Variable is not convertible to Number.");
                } else if constexpr(std::is_same_v<T, types::string::str>) {
                    throw std::runtime_error("Variable is not convertible to String.");
                } else if constexpr(std::is_same_v<T, types::table::tbl>) {
                    throw std::runtime_error("Variable is not convertible to Table.");
                } else if constexpr(std::is_same_v<T, types::nihil::nil>) {
                    throw std::runtime_error("Variable is not convertible to Nil.");
                } else if constexpr(std::is_same_v<T, types::func::data>) {
                    throw std::runtime_error("Variable is not convertible to Function.");
                } else if constexpr(std::is_same_v<T, types::file::fl>) {
                    throw std::runtime_error("Variable is not convertible to File.");
                }
            }
        }
    }
}

#endif
