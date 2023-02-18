#include "fell.hh"

int main([[maybe_unused]] int argc, char ** argv) {
    fell::lex::parse_file(argv[1]);
    fell::lang::dump_table(fell::lang::global_table);
}
