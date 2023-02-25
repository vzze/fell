#ifndef STD_HH
#define STD_HH

#include <stdexcept>
#include <iostream>

#include "variable.hh"
#include "util.hh"
#include "api.hh"
#include "lexer.hh"

namespace fell {
    namespace std {
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> general;
        void init();
    }
}

#endif
