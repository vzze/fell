#ifndef STD_HH
#define STD_HH

#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "variable.hh"
#include "util.hh"
#include "api.hh"
#include "lexer.hh"

namespace fell {
    namespace std {
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> general;
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> table;

        void init_table();
        void init();
    }
}

#endif
