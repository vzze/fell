#ifndef FILE_HH
#define FILE_HH

#include <filesystem>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <string>

namespace fell {
    namespace util {
        std::string get_file(const std::filesystem::path);
    }
}

#endif
