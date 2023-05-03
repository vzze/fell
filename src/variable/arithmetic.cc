#include <variable.hh>
#include <error.hh>

fell::var fell::var::operator + (const var & rhs) const {
    switch(type) {
        case TYPE::FUNCTION:
            throw fell::err::common("Function variable has no addition operator.");
        break;

        case TYPE::NIHIL:
            throw fell::err::common("Nihil variable has no addition operator.");
        break;

        case TYPE::INTEGER:
            switch(rhs.type) {
                case TYPE::FUNCTION:
                    throw fell::err::common("Function variable is not convertible to Integer.");
                break;

                case TYPE::NIHIL:
                    throw fell::err::common("Nihil variable is not convertible to Integer.");
                break;

                case TYPE::INTEGER:
                    return var{std::get<integer>(value) + std::get<integer>(rhs.value)};
                break;

                case TYPE::NUMBER:
                    return var{static_cast<number>(std::get<integer>(value)) + std::get<number>(rhs.value)};
                break;

                case TYPE::STRING:
                    throw fell::err::common("String variable is not convertible to Integer.");
                break;

                case TYPE::OBJECT:
                    throw fell::err::common("Object variable is not convertible to Integer.");
                break;
            }
        break;

        case TYPE::NUMBER:
            switch(rhs.type) {
                case TYPE::FUNCTION:
                    throw fell::err::common("Function variable is not convertible to Integer.");
                break;

                case TYPE::NIHIL:
                    throw fell::err::common("Nihil variable is not convertible to Number.");
                break;

                case TYPE::INTEGER:
                    return var{std::get<number>(value) + static_cast<number>(std::get<integer>(rhs.value))};
                break;

                case TYPE::NUMBER:
                    return var{std::get<number>(value) + std::get<number>(rhs.value)};
                break;

                case TYPE::STRING:
                    throw fell::err::common("String variable is not convertible to Number.");
                break;

                case TYPE::OBJECT:
                    throw fell::err::common("Object variable is not convertible to Number.");
                break;
            }
        break;

        case TYPE::STRING:
            switch(rhs.type) {
                case TYPE::FUNCTION:
                    throw fell::err::common("Function variable is not convertible to String.");
                break;

                case TYPE::NIHIL:
                    throw fell::err::common("Nihil variable is not convertible to String.");
                break;

                case TYPE::INTEGER:
                    throw fell::err::common("Integer variable is not convertible to String.");
                break;

                case TYPE::NUMBER:
                    throw fell::err::common("Number variable is not convertible to String.");
                break;

                case TYPE::STRING:
                    return var{std::get<string>(value) + std::get<string>(rhs.value)};
                break;

                case TYPE::OBJECT:
                    throw fell::err::common("Object variable is not convertible to String.");
                break;
            }
        break;

        case TYPE::OBJECT:
            throw fell::err::common("Object variable has no addition operator.");
        break;
    }

    throw fell::err::common("Poisoned variable.");
}

#define ARITHMETIC_OP(op) \
    case TYPE::INTEGER: \
        switch(rhs.type) { \
            case TYPE::FUNCTION: \
                throw fell::err::common("Function variable is not convertible to Integer."); \
            break; \
            \
            case TYPE::NIHIL: \
                throw fell::err::common("Nihil variable is not convertible to Integer."); \
            break; \
            \
            case TYPE::INTEGER: \
                return var{std::get<integer>(value) op std::get<integer>(rhs.value)}; \
            break; \
            \
            case TYPE::NUMBER: \
                return var{static_cast<number>(std::get<integer>(value)) op std::get<number>(rhs.value)}; \
            break; \
            \
            case TYPE::STRING: \
                throw fell::err::common("String variable is not convertible to Integer."); \
            break; \
            \
            case TYPE::OBJECT: \
                throw fell::err::common("Object variable is not convertible to Integer."); \
            break; \
        } \
    break; \
    \
    case TYPE::NUMBER: \
        switch(rhs.type) { \
            case TYPE::FUNCTION: \
                throw fell::err::common("Function variable is not convertible to Number."); \
            break; \
            \
            case TYPE::NIHIL: \
                throw fell::err::common("Nihil variable is not convertible to Number."); \
            break; \
            \
            case TYPE::INTEGER: \
                return var{std::get<number>(value) op static_cast<number>(std::get<integer>(rhs.value))}; \
            break; \
            \
            case TYPE::NUMBER: \
                return var{std::get<number>(value) op std::get<number>(rhs.value)}; \
            break; \
            \
            case TYPE::STRING: \
                throw fell::err::common("String variable is not convertible to Number."); \
            break; \
            \
            case TYPE::OBJECT: \
                throw fell::err::common("Object variable is not convertible to Number."); \
            break; \
        } \
    break \

fell::var fell::var::operator - (const var & rhs) const {
    switch(type) {
        case TYPE::FUNCTION:
            throw fell::err::common("Function variable has no substraction operator.");
        break;

        case TYPE::NIHIL:
            throw fell::err::common("Nihil variable has no substraction operator.");
        break;

        ARITHMETIC_OP(-);

        case TYPE::STRING:
            throw fell::err::common("String variable has no substraction operator.");
        break;

        case TYPE::OBJECT:
            throw fell::err::common("Object variable has no substraction operator.");
        break;
    }

    throw fell::err::common("Poisoned variable.");
}

fell::var fell::var::operator * (const var & rhs) const {
    switch(type) {
        case TYPE::FUNCTION:
            throw fell::err::common("Function variable has no multiplication operator.");
        break;

        case TYPE::NIHIL:
            throw fell::err::common("Nihil variable has no multiplication operator.");
        break;

        ARITHMETIC_OP(*);

        case TYPE::STRING:
            throw fell::err::common("String variable has no multiplication operator.");
        break;

        case TYPE::OBJECT:
            throw fell::err::common("Object variable has no multiplication operator.");
        break;
    }

    throw fell::err::common("Poisoned variable.");
}

fell::var fell::var::operator / (const var & rhs) const {
    switch(type) {
        case TYPE::FUNCTION:
            throw fell::err::common("Function variable has no division operator.");
        break;

        case TYPE::NIHIL:
            throw fell::err::common("Nihil variable has no division operator.");
        break;

        ARITHMETIC_OP(/);

        case TYPE::STRING:
            throw fell::err::common("String variable has no division operator.");
        break;

        case TYPE::OBJECT:
            throw fell::err::common("Object variable has no division operator.");
        break;
    }

    throw fell::err::common("Poisoned variable.");
}

fell::var fell::var::operator % (const var & rhs) const {
    switch(type) {
        case TYPE::FUNCTION:
            throw fell::err::common("Function variable has no modulus operator.");
        break;

        case TYPE::NIHIL:
            throw fell::err::common("Nihil variable has no modulus operator.");
        break;

        case TYPE::INTEGER:
            switch(rhs.type) {
                case TYPE::FUNCTION:
                    throw fell::err::common("Function variable is not convertible to Integer.");
                break;

                case TYPE::NIHIL:
                    throw fell::err::common("Nihil variable is not convertible to Integer.");
                break;

                case TYPE::INTEGER:
                    return var{std::get<integer>(value) % std::get<integer>(rhs.value)};
                break;

                case TYPE::NUMBER:
                    return var{
                        static_cast<number>(std::get<integer>(value)) -
                        std::floor(
                            static_cast<number>(std::get<integer>(value)) /
                            std::get<number>(rhs.value)
                        ) * std::get<number>(rhs.value)
                    };
                break;

                case TYPE::STRING:
                    throw fell::err::common("String variable is not convertible to Integer.");
                break;

                case TYPE::OBJECT:
                    throw fell::err::common("Object variable is not convertible to Integer.");
                break;
            }
        break;

        case TYPE::NUMBER:
            switch(rhs.type) {
                case TYPE::FUNCTION:
                    throw fell::err::common("Function variable is not convertible to Number.");
                break;

                case TYPE::NIHIL:
                    throw fell::err::common("Nihil variable is not convertible to Number.");
                break;

                case TYPE::INTEGER:
                    return var{
                        std::get<number>(value) -
                        std::floor(
                            std::get<number>(value) /
                            static_cast<number>(std::get<integer>(rhs.value))
                        ) * static_cast<number>(std::get<integer>(rhs.value))
                    };
                break;

                case TYPE::NUMBER:
                    return var{
                        std::get<number>(value) -
                        std::floor(
                            std::get<number>(value) /
                            std::get<number>(rhs.value)
                        ) * std::get<number>(rhs.value)
                    };
                break;

                case TYPE::STRING:
                    throw fell::err::common("String variable is not convertible to Number.");
                break;

                case TYPE::OBJECT:
                    throw fell::err::common("Object variable is not convertible to Number.");
                break;
            }
        break ;

        case TYPE::STRING:
            throw fell::err::common("String variable has no modulus operator.");
        break;

        case TYPE::OBJECT:
            throw fell::err::common("Object variable has no modulus operator.");
        break;
    }

    throw fell::err::common("Poisoned variable.");
}

#undef ARITHMETIC_OP
