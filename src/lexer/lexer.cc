#include "lexer.hh"
#include "util.hh"

void fell::lex::parse_file(const std::filesystem::path path) {
    std::string file;

    try {
        file = util::get_file(path);
    } catch(std::exception & e) {
        std::cout << e.what() << '\n';
    }

    fell::util::remove_comments(file);

    std::vector<std::string> statements;

    for(const auto statement : std::views::split(file, std::string_view{";"})) {
        statements.emplace_back(statement.begin(), statement.end());
    }

    statements.pop_back();

    for(auto & statement : statements) {
        try {
            auto copy = statement;
            const auto counter = check_for_string_constant(copy);

            if(copy.find(keywords::LET) != std::string::npos) {
                let(copy.substr(copy.find_first_not_of("\n\r\x0d ")));
            }

            clear_string_constants(counter);
        } catch(std::exception & e) {
            std::cout << path.filename().string() << ":\n    " << statement << '\n';
            std::cout << "    " << e.what() << '\n';
            return;
        }
    }
}
