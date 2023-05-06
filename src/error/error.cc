#include <error.hh>

fell::err::common::common(
    const std::size_t line,
    const std::size_t column,
    const std::string & str
) : std::runtime_error{"Code fell off at (" + std::to_string(line) + "/" + std::to_string(column) + "):\n" + str} {
    err = std::string("(") + std::to_string(line) + "/" + std::to_string(column) + "):\n" + str;
}

void fell::err::log(const err::common & e, const std::string_view extra) {
    std::clog << e.start << extra << e.err << '\n';
}

void fell::err::log(const std::exception & e) {
    std::clog << e.what() << '\n';
}

void fell::err::log(const std::size_t line, const std::size_t column, const std::exception & e) {
    std::clog << "Code fell off at " << '(' << line << '/' << column << "):\n" << e.what() << '\n';
}
