#ifndef LANG_HH
#define LANG_HH

#include "variable.hh"
#include "util.hh"

namespace fell {
    namespace lang {
        extern types::variable::var global_table;
        void dump_table(types::variable::var &, std::string = "");
    }
}

#endif
