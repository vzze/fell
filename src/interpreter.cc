#include "fell.hh"

int main(int argc, char ** argv) {
    fell::std::init();
    if(fell::util::parse_args(argc, argv))
        fell::lex::parse_file(argv[1]);
}
