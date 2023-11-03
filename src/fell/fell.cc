#include <fell/var/var.hh>
#include <fell/var/underlying/underlying.hh>
#include <iostream>

int main() {
    fell::var obj(fell::make_tbl());

    const fell::var key(fell::make_str("key"));

    const fell::var value(fell::num(1));

    try {
        obj[key] = value;
        std::cout << fell::get<fell::num>(obj[key]);
    } catch(...) {
        return 1;
    }

    /* fell::var some1(fell::make_str("Hello ")); */
    /* fell::var some2(fell::make_str("world!")); */

    /* try { */
    /*     std::cout << *fell::get<fell::str>(some1 + some2) << '\n'; */
    /* } catch(...) { */
    /*     return 1; */
    /* } */
}
