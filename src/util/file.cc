#include "file.hh"

std::string fell::util::get_file(const std::filesystem::path path) {
    std::ifstream f(path, std::ios::binary);

    if(!f)
        throw std::runtime_error(("File not found: " + path.string()).c_str());

    std::string content;
    f.seekg(0, std::ios::end);

    content.resize(static_cast<std::size_t>(f.tellg()));
    f.seekg(0, std::ios::beg);

    f.read(&content[0], static_cast<std::int64_t>(content.size()));
    f.close();

    return content;
}
