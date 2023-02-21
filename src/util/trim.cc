#include "util.hh"

void fell::util::trim(std::string & str) {
    std::size_t offset1 = 0;

    while(std::isspace(str[offset1]) && offset1 < str.length())
        ++offset1;

    if(offset1 == str.length()) {
        str = "";
        return;
    };

    for(std::size_t i = str.length() - 1; i >= offset1; --i)
        if(!std::isspace(str[i])) {
            str = str.substr(offset1, i + 1 - offset1);
            return;
        }

    str = str.substr(offset1);
}
