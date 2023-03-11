#include "fell.hh"

int main(int argc, char ** argv) {
    fell::std::init();

    fell::std::init_table();
    fell::std::init_io();

    if(fell::util::parse_args(argc, argv))
        fell::lex::parse_file(argv[1]);
}
