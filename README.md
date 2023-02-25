# Fell
Fell is an interpreted language inspired by the Lisp family and Lua. It is meant to be embedded into compiled languages but can be used as a standalone language.

This repository contains the source code for the interpreter, the static library and Fell's standard library.


```
fibo = |n| {
    ret = if(n <= 1,
        || { ret = n; },
        || { ret = fibo(n - 1) + fibo(n - 2); }
    );
};

print(fibo(9));
```

### Dependencies
* `g++` 12.2.0 or later with C++20 support
* `cmake` 3.2 or later

### Installation and Usage
Cloning the repository, then running cmake will generate an executable that can be used on standalone Fell code. A static library will also be generated which grants interoperability inbetween Fell code and C++ code.

Fell provides an API for C++ through `include/api.hh`, this is the same interface used in developing Fell's standard library.

#### Snippet
```c++
[](fell::api::params params) -> fell::types::variable::var {
    if(params.number_of_params() == 0)
        // .. do something
    else if(params.get_param(1).get_value<api::param::str>() == "Hello")
        return api::make_var<types::number>(42);

    return api::make_var<types::nihil>();
}
```
