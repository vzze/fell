#include "util.hh"

double fell::util::str_view_tod(std::string_view str) {
    double ret = 0.0;

    std::size_t i = 0;

    for(; i < str.length(); ++i) {
        if(str[i] == '.')
            break;

        if(!std::isdigit(str[i]))
            throw std::runtime_error{"Conversion not successful."};

        ret = ret * 10 + str[i] - '0';
    }

    std::size_t j = i;
    ++i;

    for(; i < str.length(); ++i) {
        if(!std::isdigit(str[i]))
            throw std::runtime_error{"Conversion not successful."};

        ret += static_cast<double>(str[i] - '0') / std::pow(10, i - j);
    }

    return ret;
}
