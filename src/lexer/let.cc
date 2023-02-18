#include "lexer.hh"

void fell::lex::let(std::string && line) {
    const auto cutoff = line.find_first_of("=", 4);
    if(cutoff == std::string::npos) {
        (*lang::global_table)[line.substr(4, line.find_first_of(" ", 4) - 4)] = util::make_var<types::nihil>();
    } else {
        (*lang::global_table)[line.substr(4, line.find_first_of(" =", 4) - 4)] = solve_expression(line.substr(cutoff + 1));
    }
}
