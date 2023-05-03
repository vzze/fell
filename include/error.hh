#ifndef ERROR_HH
#define ERROR_HH

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <string>

namespace fell::err {
    struct common : public std::runtime_error {
        using std::runtime_error::runtime_error;

        common(const std::size_t, const std::size_t, const std::string & str);
    };

    void log(const std::exception &);
    void log(const std::size_t, const std::size_t, const std::exception &);
}

#endif
