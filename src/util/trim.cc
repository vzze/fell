#include "util.hh"

std::string fell::util::trim(const std::string str) {
    const auto offset1 = str.find_first_not_of(' ');
    if(offset1 == std::string::npos) return "";
    return str.substr(offset1, str.find_first_of(" \n\r\f", offset1) - offset1);
}
