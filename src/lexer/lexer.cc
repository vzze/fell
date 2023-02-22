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

    std::vector<std::string_view> statements;

    for(const auto statement : std::views::split(file, std::string_view{";"})) {
        statements.emplace_back(statement.begin(), statement.end());
    }

    statements.pop_back();

    for(auto & statement : statements) {
        try {
            util::trim(statement);

            std::size_t check_def;

            if((check_def = statement.find(keywords::LET)) != std::string::npos) {
                if(check_def != 0)
                    throw std::runtime_error{"Extra keyword in definition: " + std::string{statement.substr(0, check_def)}};
                let(statement);
            }
        } catch(std::exception & e) {
            std::cout << path.filename().string() << ":\n    " << statement << '\n';
            std::cout << "    " << e.what() << '\n';
            return;
        }
    }
}
