#include "lexer.hh"
#include "util.hh"

#include <iostream>

void fell::lex::parse_file(const std::filesystem::path path) {
    std::string file;

    try {
        file = util::get_file(path);
    } catch(std::exception & e) {
        std::cout << e.what() << '\n';
    }

    fell::util::remove_comments(file);

    for(std::size_t i = 1; const auto _line : std::views::split(file, std::string_view{"\n"})) {
        const std::string_view line{_line.begin(), _line.end()};

        try {
            if(line.find(keywords::LET) != std::string_view::npos) {
                let(std::string{line.begin(), line.end()});
            } else {

            }
        } catch(std::exception & e) {
            std::cout << path.filename().string() << " -> L" << i << ": " << line << '\n';
            std::cout << "    " << e.what() << '\n';
            return;
        }

        ++i;
    }
}
