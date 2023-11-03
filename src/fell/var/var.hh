#ifndef FELL_VAR_HH
#define FELL_VAR_HH

#include <cmath>

#include <fell/var/underlying/underlying.hh>
#include <fell/var/helpers/concepts.hh>
#include <fell/var/helpers/errors.hh>

namespace fell {
    struct var {
        public:
            using stored = std::variant<
                nil, num, str, arr, tbl
            >;
        private:
            stored data;
        public:
            var() = default;
            // NOLINTNEXTLINE (bogus warning concept is constrained)
            explicit var(concepts::held auto && stored)
                : data(std::forward<decltype(stored)>(stored)) {}

            friend decltype(auto) visit(
                concepts::callable auto && vis,
                concepts::variable auto && var
            );

            template<concepts::underlying U>
            friend decltype(auto) get(concepts::variable auto && var);

            var & operator [] (concepts::variable auto && rhs);
    };

    var operator * (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator / (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator % (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator - (concepts::variable auto && lhs, concepts::variable auto && rhs);

    var operator + (concepts::variable auto && lhs, concepts::variable auto && rhs);

    var operator == (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator <= (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator <  (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator >= (concepts::variable auto && lhs, concepts::variable auto && rhs);
    var operator >  (concepts::variable auto && lhs, concepts::variable auto && rhs);
}

decltype(auto) fell::visit(concepts::callable auto && vis, concepts::variable auto && var) {
    return std::visit(
        std::forward<decltype(vis)>(vis),
        std::forward<decltype(var)>(var).data
    );
}

template<fell::concepts::underlying U>
decltype(auto) fell::get(concepts::variable auto && var) {
    return std::get<U>(std::forward<decltype(var)>(var).data);
}

fell::var & fell::var::operator [] (concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](fell::concepts::array auto && lhs) -> var & {
            return visit(concepts::visitor{
                [&](fell::concepts::number auto && v_rhs) -> var & {
                    return (*lhs)[static_cast<std::size_t>(v_rhs)];
                },
                [](auto && lhs) -> var & {
                    throw err::var::cannot_be_used_as_index<decltype(lhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](fell::concepts::table auto && lhs) -> var & {
            return visit(concepts::visitor{
                [&](fell::concepts::number auto && v_rhs) -> var & {
                    return (*lhs)[v_rhs];
                },
                [&](fell::concepts::string auto && v_rhs) -> var & {
                    return (*lhs)[*v_rhs];
                },
                [](auto && lhs) -> var & {
                    throw err::var::cannot_be_used_as_key<decltype(lhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && lhs) -> var & {
            throw err::var::has_no_operator<decltype(lhs)>("subscript");
        }
    }, *this);
}

fell::var fell::operator * (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var { return var(v_lhs * v_rhs); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("*"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var { throw err::var::has_no_operator<decltype(v_lhs)>("*"); }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator / (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var { return var(v_lhs / v_rhs); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("/"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var { throw err::var::has_no_operator<decltype(v_lhs)>("/"); }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator % (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var { return var(std::fmod(v_lhs, v_rhs)); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("%"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var { throw err::var::has_no_operator<decltype(v_lhs)>("%"); }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator - (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var { return var(v_lhs - v_rhs); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("-"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var { throw err::var::has_no_operator<decltype(v_lhs)>("-"); }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator + (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var { return var(v_lhs + v_rhs); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("+"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var { return var(make_str(*v_lhs + *v_rhs)); },
                [](auto && v_rhs) -> var { throw err::var::has_no_operator<decltype(v_rhs)>("+"); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var { throw err::var::has_no_operator<decltype(v_lhs)>("+"); }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator == (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::nihil auto &&  /* v_lhs */) -> var {
            return visit(concepts::visitor{
                [](concepts::nihil auto && /* v_rhs */) -> var { return var(num(1)); },
                [](auto && /* unused */) -> var { return var(num(0)); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var {
                    return var(num(v_lhs == v_rhs));
                },
                [](auto && /* unused */) -> var { return var(num(0)); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var {
                    return var(num(*v_lhs == *v_rhs));
                },
                [](auto && /* unused */) -> var { return var(num(0)); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::array auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::array auto && v_rhs) -> var {
                    return var(num(v_lhs.get() == v_rhs.get()));
                },
                [](auto && /* unused */) -> var { return var(num(0)); }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::table auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::table auto && v_rhs) -> var {
                    return var(num(v_lhs.get() == v_rhs.get()));
                },
                [](auto && /* unused */) -> var { return var(num(0)); }
            }, std::forward<decltype(rhs)>(rhs));
        }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator <= (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var {
                    return var(num(v_lhs <= v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var {
                    return var(num(*v_lhs <= *v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var {
            throw err::var::not_comparable<decltype(v_lhs)>();
        }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator < (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var {
                    return var(num(v_lhs < v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var {
                    return var(num(*v_lhs < *v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var {
            throw err::var::not_comparable<decltype(v_lhs)>();
        }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator >= (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var {
                    return var(num(v_lhs >= v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var {
                    return var(num(*v_lhs >= *v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var {
            throw err::var::not_comparable<decltype(v_lhs)>();
        }
    }, std::forward<decltype(lhs)>(lhs));
}

fell::var fell::operator > (concepts::variable auto && lhs, concepts::variable auto && rhs) {
    return visit(concepts::visitor{
        [&](concepts::number auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::number auto && v_rhs) -> var {
                    return var(num(v_lhs > v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [&](concepts::string auto && v_lhs) -> var {
            return visit(concepts::visitor{
                [&](concepts::string auto && v_rhs) -> var {
                    return var(num(*v_lhs > *v_rhs));
                },
                [](auto && v_rhs) -> var {
                    throw err::var::not_comparable_with<decltype(v_lhs), decltype(v_rhs)>();
                }
            }, std::forward<decltype(rhs)>(rhs));
        },
        [](auto && v_lhs) -> var {
            throw err::var::not_comparable<decltype(v_lhs)>();
        }
    }, std::forward<decltype(lhs)>(lhs));
}

#endif
