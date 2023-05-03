#include <compiler.hh>

void fell::compiler::constant_integer(
    const scan::scanned & data,
    std::size_t & i,
    std::size_t & count,
    bool & alternance,
    instructions & instructions,
    std::stack<vm::INSTRUCTIONS> & operators
) {
    if(alternance == true) throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected constant.");
    alternance = true;

    const auto it = std::find_if(constants.begin(), constants.end(), [&](const var & el) -> bool {
        if(el.get_type() == var::TYPE::INTEGER) {
            if(el.get<var::integer>() == data.constant_integers[count])
                return true;
            else
                return false;
        } else {
            return false;
        }
    });

    if(it != constants.end()) {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(it - constants.begin()));
    } else {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
        constants.emplace_back(data.constant_integers[count]);
    }

    operators.push(vm::INSTRUCTIONS::LOC);
    ++count;
}

void fell::compiler::constant_float(
    const scan::scanned & data,
    std::size_t & i,
    std::size_t & count,
    bool & alternance,
    instructions & instructions,
    std::stack<vm::INSTRUCTIONS> & operators
) {
    if(alternance == true) throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected constant.");
    alternance = true;

    auto it = std::find_if(constants.begin(), constants.end(), [&](const var & el) -> bool {
        if(el.get_type() == var::TYPE::NUMBER) {
            if(el.get<var::number>() == data.constant_floatings[count])
                return true;
            else
                return false;
        } else {
            return false;
        }
    });

    if(it != constants.end()) {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(it - constants.begin()));
    } else {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
        constants.emplace_back(data.constant_floatings[count]);
    }


    operators.push(vm::INSTRUCTIONS::LOC);
    ++count;
}

void fell::compiler::constant_string(
    const scan::scanned & data,
    std::size_t & i,
    std::size_t & count,
    bool & alternance,
    instructions & instructions,
    std::stack<vm::INSTRUCTIONS> & operators
) {
    if(alternance == true) throw err::common(data.locations[i].line, data.locations[i].column, "Unexpected constant.");
    alternance = true;

    auto it = std::find_if(constants.begin(), constants.end(), [&](const var & el) -> bool {
        if(el.get_type() == var::TYPE::STRING) {
            if(el.get<var::string>() == data.constant_strings[count])
                return true;
            else
                return false;
        } else {
            return false;
        }
    });

    if(it != constants.end()) {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(it - constants.begin()));
    } else {
        instructions.top(true, i)->push_back(static_cast<std::int32_t>(constants.size()));
        constants.emplace_back(data.constant_strings[count]);
    }


    operators.push(vm::INSTRUCTIONS::LOC);
    ++count;
}
