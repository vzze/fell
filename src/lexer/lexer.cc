#include "lexer.hh"

fell::lex::inmemory::inmemory(types::variable::var && v) : non_reference{std::move(v)}, reference{nullptr} {}
fell::lex::inmemory::inmemory(types::variable::var * v) : non_reference{nullptr}, reference{v} {}

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
            solve_expression(statement);
        } catch(std::exception & e) {
            std::cout << path.filename().string() << ":\n" << statement << '\n';
            std::cout << "->> " << e.what() << '\n';
            return;
        }
    }
}
