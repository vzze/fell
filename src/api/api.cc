#include "api.hh"

fell::api::param::param(fell::types::variable::var & var) : v(var) {}

fell::types::variable::var & fell::api::param::expose() {
    return v;
}

fell::api::params::params(std::vector<lex::inmemory> && pars) : pars{std::move(pars)} {};

fell::api::param fell::api::params::get_param(std::size_t index) {
    if(pars.at(index).non_reference)
        return param{pars.at(index).non_reference};
    else
        return param{*pars.at(index).reference};
}

void fell::api::params::for_each_param(std::function<void(param)> applied) {
    for(auto & p : pars) {
        if(p.non_reference)
            applied(param{p.non_reference});
        else
            applied(param{*p.reference});
    }
}

std::size_t fell::api::params::number_of_params() {
    return pars.size();
}

fell::types::variable::var fell::api::make_func(std::function<types::variable::var(params)> fn) {
    return util::make_var<types::func>(types::func::data({}, "", fn));
}

void fell::api::set_global_var(std::string name, types::variable::var var) {
    (*lex::global_table)[name] = std::move(var);
}
