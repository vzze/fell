#include <vm.hh>

fell::var fell::vm::call(fell::var & vr, std::vector<var*> params) {
    stack_frame.push_back(memory.size());

    program.push({0, &labels[std::get<std::size_t>(vr.get<var::func>())], vm::INSTRUCTIONS::CAL, runtime.size()});

    for(var * param : params)
        memory.emplace_back(param);

    return run(program.size() - 1);
}

fell::var fell::vm::call(std::pair<std::vector<scan::location>, std::vector<std::int32_t>> & module) {
    stack_frame.push_back(memory.size());

    program.push({0, &module, vm::INSTRUCTIONS::CAL, runtime.size()});

    return run(program.size() - 1);
}
