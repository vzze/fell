#include "lexer.hh"

void fell::lex::let(std::string line) {
    const auto cutoff = line.find_first_of("=", 4);
    if(cutoff == std::string::npos) {
        const auto name = line.substr(4, line.find_first_of(" \x0d", 4) - 4);
        check_for_invalid_redefinition(name);

        (*lang::global_table)[name] = util::make_var<types::nihil>();
    } else {
        const auto name = line.substr(4, line.find_first_of(" =", 4) - 4);
        check_for_invalid_redefinition(name);

        (*lang::global_table)[name] = solve_expression(line.substr(cutoff + 1));
    }
}

void fell::lex::check_for_invalid_redefinition(const std::string & name) {
    if(name == keywords::FALSE || name == keywords::TRUE || name == keywords::LET || name == keywords::NIHIL)
        throw std::runtime_error("Attempt to redefine keyword as different kind of symbol.");
    if(name.find("__string_constant_") == 0)
        throw std::runtime_error("Expression is not assignable.");

    try {
        [[maybe_unused]] const auto s = std::stod(name);
    } catch(...) { return; }

    throw std::runtime_error("Expression is not assignable");
}
