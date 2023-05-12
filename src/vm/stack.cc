#include <vm.hh>

void fell::vm::stack::init() {
    data.resize(10000);
}

void fell::vm::stack::pop() {
    --_size;
}

fell::vm::holder & fell::vm::stack::top() {
    return data[_size - 1];
}

std::size_t fell::vm::stack::size() const {
    return _size;
}

bool fell::vm::stack::empty() const {
    return _size == 0;
}

void fell::vm::stack::emplace(std::size_t && index, holder::TYPE && type) {
    data[_size].type = std::move(type);
    data[_size].value = std::move(index);
    ++_size;
}

void fell::vm::stack::emplace(var & v, holder::TYPE && type) {
    data[_size].type = std::move(type);
    data[_size].value = v;
    ++_size;
}

void fell::vm::stack::emplace(var && v, holder::TYPE && type) {
    data[_size].type = std::move(type);
    data[_size].value = std::move(v);
    ++_size;
}

void fell::vm::stack::emplace(var * v) {
    data[_size].value = v;
    data[_size].type = vm::holder::TYPE::OBJ_PROP;
    ++_size;
}
