#ifndef DEBUG_HH
#define DEBUG_HH

#include <iomanip>
#include <iostream>

#include <scanner.hh>
#include <vm.hh>

namespace fell::debug {
    void scanner(const scan::scanned &);
    void compiler(const vm &);
    void vm_memory(vm &);
}

#endif
