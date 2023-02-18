#include "lexer.hh"

#include <iostream>

void fell::lex::parse_file(const std::filesystem::path path) {
    const auto file = util::get_file(path);


    for(std::size_t i = 1; const auto _line : std::views::split(file, std::string_view{"\n"})) {
        const std::string_view line{_line.begin(), _line.end()};

        try {
            if(line.find(keywords::LET) != std::string_view::npos) {
                let(std::string{line.begin(), line.end()});
            } else {

            }
        } catch(std::exception & e) {
            std::cout << "LINE " << i << ": " << e.what() << '\n';
            return;
        }

        ++i;
    }
}
