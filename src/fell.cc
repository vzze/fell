#include <fell.hh>

int main(int argc, char ** argv) {
    if(argc > 1) { fell::interpreter{argv[1]}; }
    return 0;
}
