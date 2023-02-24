#include "fell.hh"

int main([[maybe_unused]] int argc, char ** argv) {
    fell::std::init();
    fell::lex::parse_file(argv[1]);
}
