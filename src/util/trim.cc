#include "util.hh"

std::string fell::util::trim(const std::string str) {
    const auto offset1 = str.find_first_not_of("\n\r\x0d ");
    if(offset1 == std::string::npos) return "";
    for(std::size_t i = str.length() - 1; i >= offset1; --i)
        if(str[i] != ' ' && str[i] != '\r' && str[i] != '\n' && str[i] != '\x0d')
            return str.substr(offset1, i + 1 - offset1);
    return str.substr(offset1);
}
