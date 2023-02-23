#include "lang.hh"

fell::types::variable::var fell::lang::global_table = fell::util::make_var<fell::types::table>();
std::vector<fell::types::variable::var> fell::lang::contexts = {};
