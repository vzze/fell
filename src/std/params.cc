#include <std.hh>

std::size_t fell::lib::params::number() const {
    return size;
}

void fell::lib::params::for_each(std::function<void (var &)> f, const std::size_t o) {
    for(std::size_t i = offset + o; i < offset + size; ++i)
        f(vm->get(vm->memory[i]));
}

fell::var & fell::lib::params::operator [] (const std::size_t i) {
    if(i >= size) throw err::common("Embedded function expected at least " + std::to_string(i + 1) + " parameters.");
    return vm->get(*(vm->memory.begin() + static_cast<std::int64_t>(i) + static_cast<std::int64_t>(offset)));
}

fell::var fell::lib::params::call_function(var & vr, std::vector<var*> params) {
    return vm->call(vr, params);
}

fell::vm::stack & fell::lib::params::get_stack() {
    return vm->runtime;
}

void fell::lib::params::pop_stack_frame() {
    return vm->stack_frame.pop_back();
}

std::filesystem::path & fell::lib::params::cwd() {
    return vm->cwd;
}

fell::var fell::lib::params::call_module(std::pair<std::vector<scan::location>, std::vector<std::int32_t>> & module) {
    return vm->call(module);
}

fell::lib::params::params(fell::vm * v, const std::size_t off, const std::size_t sz) : offset{off}, size{sz}, vm{v} {}
