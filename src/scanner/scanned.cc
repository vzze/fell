#include <scanner.hh>

void fell::scan::scanned::push_token(const TOKENS token, const location loc) {
    locations.push_back(loc);
    tokens.push_back(token);
}

void fell::scan::scanned::push_identifier(const var::string id, const location loc) {
    locations.push_back(loc);
    tokens.push_back(TOKENS::IDENTIFIER);
    identifiers.push_back(id);
}

void fell::scan::scanned::push_constant(const var::integer constant, const location loc) {
    locations.push_back(loc);
    tokens.push_back(TOKENS::INTEGER);
    constant_integers.push_back(constant);
}

void fell::scan::scanned::push_constant(const var::number constant, const location loc) {
    locations.push_back(loc);
    tokens.push_back(TOKENS::NUMBER);
    constant_floatings.push_back(constant);
}

void fell::scan::scanned::push_constant(const var::string constant, const location loc) {
    locations.push_back(loc);
    tokens.push_back(TOKENS::STRING);
    constant_strings.push_back(constant);
}
