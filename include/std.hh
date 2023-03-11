#ifndef STD_HH
#define STD_HH

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <list>

#include "variable.hh"
#include "util.hh"
#include "api.hh"
#include "lexer.hh"

extern "C" {
    #include "file.h"
}

namespace fell {
    namespace std {
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> general;
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> table;
        extern ::std::vector<::std::pair<::std::string,::std::function<types::variable::var(api::params)>>> io;

        enum class FILE : ::std::int16_t {
            STDIN  = -0xFC,
            STDOUT = -0xC0,
            STDERR = -0xFF
        };

        void init_table();
        void init_io();

        void init();
    }
}

#endif
