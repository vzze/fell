#include "lexer.hh"

fell::types::variable::var fell::lex::global_table = util::make_var<fell::types::table>();
std::vector<std::unordered_map<std::string, fell::lex::inmemory>> fell::lex::contexts;
fell::lex::inmemory::inmemory() : non_reference(nullptr), reference(nullptr) {}
fell::lex::inmemory::inmemory(types::variable::var && v) : non_reference{std::move(v)}, reference{nullptr} {}
fell::lex::inmemory::inmemory(types::variable::var * v) : non_reference{nullptr}, reference{v} {}

void fell::lex::eval_code(const std::string & code) {
    for(std::size_t i = 0; i < code.length(); ++i) {
        try {
            i += solve_expression(std::string_view{code.begin() + static_cast<std::int64_t>(i), code.end()});
        } catch(std::exception & e) {
            std::cout << "->> " << e.what() << '\n';
            return;
        }
    }
}

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

    eval_code(file);
}
