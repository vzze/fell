#include "lexer.hh"

void fell::lex::parse_file(const std::filesystem::path path) {
    const auto file = util::get_file(path);

    for(const auto line : std::ranges::views::split(file, "\n")) {
    }
}
