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
Cloning the repository then running cmake will generate an executable that can be used on standalone Fell code. A static library will also be generated which grants interoperability between Fell code and C++ code.

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

#### Disclaimer
Fell is not JIT and does not generate bytecode, being a syntax tree walker it's inherently slower than an interpreted language that generates bytecode, hence the "Fast" in the about section.

The Standard Library is missing features (i.e. string manipulation)

This is a personal project that was made for fun, and I don't expect to maintain it for long periods of time.

As of commit [80b96e9](https://github.com/vzze/fell/commit/80b96e92d2c9217970d62e33c23548d21712bc9e) Fell is Turing complete (`tests/turing_completeness.fell`)
