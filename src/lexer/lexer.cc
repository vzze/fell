#include "lexer.hh"

void fell::lex::parse_file(const std::filesystem::path path) {
    const auto file = util::get_file(path);

    for(const auto _line : std::ranges::views::split(file, "\n")) {
        const std::string_view line{_line.begin(), _line.end()};
        if(line.find(keywords::LET)) {
            let(std::string{line});
        } else {

        }
    }
}
