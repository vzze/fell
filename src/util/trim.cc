#include "util.hh"

std::string fell::util::trim(const std::string str) {
    std::size_t offset1 = 0;
    while(std::isspace(str[offset1]))
        ++offset1;

    if(offset1 == std::string::npos) return "";
    for(std::size_t i = str.length() - 1; i >= offset1; --i)
        if(!std::isspace(str[i]))
            return str.substr(offset1, i + 1 - offset1);
    return str.substr(offset1);
}
