#include <std.hh>
#include <debug.hh>
#include <scanner.hh>
#include <compiler.hh>

std::size_t fell::lib::params::number() const {
    return size;
}

void fell::lib::params::for_each(std::function<void (var &)> f, const std::size_t o) {
    for(std::size_t i = offset + o; i < offset + size; ++i)
        f(vm->get(vm->memory[i]));
}

fell::var & fell::lib::params::operator [] (const std::size_t i) {
    if(i >= size) throw err::common("Embedded function expected at least " + std::to_string(i + 1) + " parameters.");
    return vm->get(*(vm->memory.begin() + static_cast<std::int64_t>(i) + static_cast<std::int64_t>(offset)));
}

fell::var fell::lib::params::call_function(var & vr, std::vector<var*> params) {
    return vm->call(vr, params);
}

fell::vm::stack & fell::lib::params::get_stack() {
    return vm->runtime;
}

void fell::lib::params::pop_stack_frame() {
    return vm->stack_frame.pop_back();
}

std::filesystem::path & fell::lib::params::cwd() {
    return vm->cwd;
}

fell::lib::params::params(fell::vm * v, const std::size_t off, const std::size_t sz) : offset{off}, size{sz}, vm{v} {}

std::vector<std::pair<std::string, std::function<fell::var(fell::lib::params)>>> fell::lib::general = {
    {
        "print",
        [](params params) -> fell::var {
            params.for_each([](var & p) {
                switch(p.get_type()) {
                    case fell::var::TYPE::FUNCTION:
                        std::cout << "fun ";
                    break;

                    case fell::var::TYPE::OBJECT:
                        std::cout << "obj ";
                    break;

                    case fell::var::TYPE::NIHIL:
                        std::cout << "nil ";
                    break;

                    case fell::var::TYPE::NUMBER:
                        std::cout << p.get<var::number>() << ' ';
                    break;

                    case fell::var::TYPE::INTEGER:
                        std::cout << p.get<var::integer>() << ' ';
                    break;

                    case fell::var::TYPE::STRING:
                        std::cout << p.get<var::string>() << ' ';
                    break;
                }
            });

            std::cout << '\n';

            return fell::var{};
        }
    },
    {
        "type",
        [](params params) -> fell::var {
            switch(params[0].get_type()) {
                case fell::var::TYPE::FUNCTION:
                    return fell::var{"fun"};
                break;

                case fell::var::TYPE::OBJECT:
                    return fell::var{"obj"};
                break;

                case fell::var::TYPE::NIHIL:
                    return fell::var{"nil"};
                break;

                case fell::var::TYPE::NUMBER:
                    return fell::var{"num"};
                break;

                case fell::var::TYPE::INTEGER:
                    return fell::var{"int"};
                break;

                case fell::var::TYPE::STRING:
                    return fell::var{"str"};
                break;
            }

            return fell::var{""};
        }
    },
    {
        "assert",
        [](params params) -> fell::var {
            if(!(bool)(params[0])) {
                throw err::common(
                    "Assertion failed: " + params[1].get<var::string>()
                );
            }

            return fell::var{};
        }
    },
    {
        "tonumber",
        [](params params) -> fell::var {
            return fell::var{
                fell::var::number{
                    std::stod(params[0].get<var::string>())
                }
            };
        }
    },
    {
        "tointeger",
        [](params params) -> fell::var {
            return fell::var{fell::var::integer{std::stoi(params[0].get<var::string>())}};
        }
    },
    {
        "tostring",
        [](params params) -> fell::var {
            switch(params[0].get_type()) {
                case var::TYPE::INTEGER:
                    return std::to_string(params[0].get<var::integer>());
                break;

                case var::TYPE::NUMBER:
                    return std::to_string(params[0].get<var::number>());
                break;

                default:
                    throw fell::err::common("Expected Number or Integer.");
                break;
            }
        }
    },
    {
        "for_each",
        [](params params) -> fell::var {
            switch(params[0].get_type()) {
                case var::TYPE::OBJECT:
                    for(var & v : params[0].get<var::object>().second)
                        params.call_function(params[1], {&v});

                    return var::nihil{};
                break;

                default:
                    throw fell::err::common("Expected Object.");
                break;
            }
       }
    },
    {
        "for_each_kv",
        [](params params) -> fell::var {
            switch(params[0].get_type()) {
                case var::TYPE::OBJECT:
                    for(auto & [k, v] : params[0].get<var::object>().first) {
                        var key = k;
                        params.call_function(params[1], {&key, &v});
                    }

                    return var::nihil{};
                break;

                default:
                    throw fell::err::common("Expected Object.");
                break;
            }
        }
    },
    {
        "error",
        [](params params) -> fell::var {
            throw fell::err::common(params[0].get<var::string>());
        }
    },
    {
        "protected_call",
        [](params params) -> fell::var {
            std::vector<var*> modf;

            params.for_each([&](var & v) {
                modf.push_back(&v);
            }, 1);

            const std::size_t snapshot = params.get_stack().size();

            try {
                auto ret = params.call_function(params[0], modf);
                return var{var::object{
                    {
                        { "err", var::integer{0} },
                        { "ret", ret }
                    },
                    {}
                }};
            } catch(...) {
                while(snapshot < params.get_stack().size())
                    params.get_stack().pop();

                params.pop_stack_frame();

                return var::object{
                    {
                        { "err", var::integer{1} },
                        { "ret", var::nihil{} }
                    },
                    {}
                };
            }
        }
    },
    {
        "require",
        [](params params) -> fell::var {
            try {
                vm vm;
                const auto file = params.cwd() / params[0].get<var::string>();
                vm.cwd = file.parent_path();

                {
                    const auto data = scan::file(file);
                    fell::compiler::process(data, vm);
                }

                return vm.main();
            } catch(const std::exception & e) {
                throw err::common(
                    "In module " +
                    (params.cwd() / params[0].get<var::string>()).string() + "\n" + e.what()
                );
            }
        }
    },
    {
        "read",
        [](params params) -> fell::var {
            if(params[0].get_type() != var::TYPE::STRING)
                throw fell::err::common("Expected String.");

            if(params[0].get<var::string>() == "int") {
                var::integer x;
                std::cin >> x;
                return x;
            } else if(params[0].get<var::string>() == "num") {
                var::number x;
                std::cin >> x;
                return x;
            } else if(params[0].get<var::string>() == "str") {
                var::string x;
                std::cin >> x;
                return x;
            } else if(params[0].get<var::string>() == "line") {
                var::string x;
                std::getline(std::cin, x);
                return x;
            } else {
                throw fell::err::common("Unknown read method.");
            }
        }
    }
};
