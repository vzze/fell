#include "lexer.hh"

void fell::lex::parse_file(const std::filesystem::path path) {
    std::string file;

    try {
        file = util::get_file(path);
    } catch(std::exception & e) {
        std::cout << e.what() << '\n';
    }

    try {
        util::check_paren(file);
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
            util::trim(statement);

            const auto counter = check_for_string_constant(statement);

            if(statement.find(keywords::LET) != std::string::npos) {
                let(statement);
            }

            clear_string_constants(counter);
        } catch(std::exception & e) {
            std::cout << path.filename().string() << ":\n    " << statement << '\n';
            std::cout << "    " << e.what() << '\n';
            return;
        }
    }
}
