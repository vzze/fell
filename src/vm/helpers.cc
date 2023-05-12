#include <vm.hh>

fell::vm::holder::holder(std::size_t && v, TYPE && t) : type{std::move(t)}, value{std::move(v)} {}
fell::vm::holder::holder(var * var) : type{TYPE::OBJ_PROP}, value{var} {}
fell::vm::holder::holder(var & var, TYPE && t) : type{std::move(t)}, value{var} {}
fell::vm::holder::holder(var && var, TYPE && t) : type(std::move(t)), value{std::move(var)} {}

std::unordered_map<std::string, fell::var> fell::exposed = {};
std::vector<fell::var> fell::constants;
std::vector<std::pair<std::vector<fell::scan::location>, std::vector<std::int32_t>>> fell::labels;

fell::var & fell::vm::get(holder & h) {
    switch(h.type) {
        case holder::TYPE::REFERENCE:
            while(memory[std::get<std::size_t>(h.value)].type == holder::TYPE::REFERENCE) {
                h.value = std::get<std::size_t>(memory[std::get<std::size_t>(h.value)].value);
            }

            return get(memory[std::get<std::size_t>(h.value)]);
        break;

        case holder::TYPE::CONSTANT:
            return constants[std::get<std::size_t>(h.value)];
        break;

        case holder::TYPE::EXPOSED:
            return exposed[constants[std::get<std::size_t>(h.value)].get<var::string>()];
        break;

        case holder::TYPE::VALUE:
        case holder::TYPE::UNINITIALIZED:
            return std::get<var>(h.value);
        break;

        case holder::TYPE::OBJ_PROP:
            return *std::get<var*>(h.value);
        break;
    }

    throw fell::err::common(0, 0, "Something really bad happened.");
}
