#include "util.hh"

std::string fell::util::trim(const std::string str) {
    const auto offset1 = str.find_first_not_of(' ');
    if(offset1 == std::string::npos) return "";
    for(std::size_t i = str.length() - 1; i >= offset1; --i)
        if(str[i] != ' ' && str[i] != '\r')
            return str.substr(offset1, i + 1 - offset1);
    return str.substr(offset1);
}
