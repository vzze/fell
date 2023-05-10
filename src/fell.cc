#include <fell.hh>

int main(int argc, char ** argv) {
    if(argc > 1) return fell::interpreter{argv[1]}.cpp_return();
    return 0;
}
