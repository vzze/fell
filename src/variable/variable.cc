#include <variable.hh>
#include <error.hh>

fell::var::var(const integer & i) : type{TYPE::INTEGER }, value{i} {}
fell::var::var(const number  & n) : type{TYPE::NUMBER  }, value{n} {}
fell::var::var(const string  & s) : type{TYPE::STRING  }, value{s} {}
fell::var::var(const object  & o) : type{TYPE::OBJECT  }, value{o} {}
fell::var::var(const func    & f) : type{TYPE::FUNCTION}, value{f} {}
fell::var::var(const nihil   & n) : type{TYPE::NIHIL   }, value{n} {}

fell::var::var(integer && i) : type{TYPE::INTEGER }, value{std::move(i)} {}
fell::var::var(number  && n) : type{TYPE::NUMBER  }, value{std::move(n)} {}
fell::var::var(string  && s) : type{TYPE::STRING  }, value{std::move(s)} {}
fell::var::var(object  && o) : type{TYPE::OBJECT  }, value{std::move(o)} {}
fell::var::var(func    && f) : type{TYPE::FUNCTION}, value{std::move(f)} {}
fell::var::var(nihil   && n) : type{TYPE::NIHIL   }, value{std::move(n)} {}

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

    throw err::common("Poisoned variable.");
}
