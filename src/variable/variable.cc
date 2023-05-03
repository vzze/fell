#include <variable.hh>
#include <error.hh>

fell::var::var(const integer i) {
    value = i;
    type = TYPE::INTEGER;
}

fell::var::var(const number n) {
    value = n;
    type = TYPE::NUMBER;
}

fell::var::var(const string s) {
    value = s;
    type = TYPE::STRING;
}

fell::var::var(const object o) {
    value = o;
    type = TYPE::OBJECT;
}

fell::var::var(const func f) {
    value = f;
    type = TYPE::FUNCTION;
}

fell::var::var(const nihil n) {
    value = n;
    type = TYPE::NIHIL;
}

fell::var::TYPE fell::var::get_type() const {
    return type;
}

fell::var * fell::var::operator [] (const var & var) {
    using enum TYPE;

    switch(type) {
        case FUNCTION:
            throw err::common("Function has no subcript operator.");
        break;
        case INTEGER:
            throw err::common("Integer has no subcript operator.");
        break;

        case NUMBER:
            throw err::common("Number has no subcript operator.");
        break;

        case STRING:
            throw err::common("String has no subcript operator.");
        break;

        case NIHIL:
            throw err::common("Nihil has no subcript operator.");
        break;

        case OBJECT:
            switch(var.get_type()) {
                case INTEGER:
                    if(static_cast<std::size_t>(var.get<var::integer>()) >= std::get<var::object>(value).second.size()) {
                        std::get<var::object>(value).second.resize(
                            static_cast<std::size_t>(var.get<var::integer>() + 1)
                        );
                    }

                    return &std::get<var::object>(value).second[
                        static_cast<std::size_t>(var.get<var::integer>())
                    ];
                break;

                case NUMBER:
                    if(static_cast<std::size_t>(var.get<var::number>()) >= std::get<var::object>(value).second.size())
                        std::get<var::object>(value).second.resize(
                            static_cast<std::size_t>(var.get<var::number>() + 1)
                        );

                    return &std::get<var::object>(value).second[
                        static_cast<std::size_t>(var.get<var::number>())
                    ];
                break;

                case STRING:
                    return &std::get<var::object>(value).first[var.get<var::string>()];
                break;

                case OBJECT:
                    throw err::common("Object can't be a key for a variable of type Object.");
                break;

                case NIHIL:
                    throw err::common("Nihil can't be a key for a variable of type Object.");
                break;

                case FUNCTION:
                    throw err::common("Function can't be a key for a variable of type Object.");
                break;
            }
        break;
    }

    throw err::common("");
}
