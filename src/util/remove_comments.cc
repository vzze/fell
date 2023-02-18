#include "util.hh"

void fell::util::remove_comments(std::string & file) {
    std::size_t offset = 0;

    while((offset = file.find_first_of('#', offset)) != std::string::npos) {
        file.erase(offset, file.find_first_of('\n', offset) - offset);
    }
}
