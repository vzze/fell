#include <variable.hh>
#include <error.hh>

fell::var::operator bool() const {
    switch(type) {
        case TYPE::FUNCTION:
            return true;
        break;
        case TYPE::NIHIL:
            return false;
        break;
        case TYPE::INTEGER:
            return std::get<integer>(value) != integer{0};
        break;
        case TYPE::NUMBER:
            return std::get<number>(value) != number{0.0};
        break;
        case TYPE::STRING:
            return true;
        break;
        case TYPE::OBJECT:
            return true;
        break;
    }

    throw err::common("Poisoned variable.");
}

fell::var fell::var::operator || (const var & rhs) const {
    return var{
        integer{bool(*this) || bool(rhs)}
    };
}

fell::var fell::var::operator && (const var & rhs) const {
    return var{
        integer{bool(*this) && bool(rhs)}
    };
}

#define LOGICAL_OP(op) \
    fell::var fell::var::operator op (const var & rhs) const { \
        switch(type) { \
            case TYPE::NIHIL: \
                throw err::common("Nihil variable can't be compared."); \
            break; \
            \
            case TYPE::INTEGER: \
                switch(rhs.type) { \
                    case TYPE::INTEGER: \
                        return var{ \
                            integer{std::get<integer>(value) op std::get<integer>(rhs.value)} \
                        }; \
                    break; \
                    \
                    case TYPE::NUMBER: \
                        return var{ \
                            integer{static_cast<number>(std::get<integer>(value)) op std::get<number>(rhs.value)} \
                        }; \
                    break; \
                    \
                    case TYPE::NIHIL: \
                        throw err::common("Nihil variable is not convertible to Integer."); \
                    break; \
                    \
                    case TYPE::STRING: \
                        throw err::common("String variable is not convertible to Integer."); \
                    break; \
                    \
                    case TYPE::OBJECT: \
                        throw err::common("Object variable is not convertible to Integer."); \
                    break; \
                    \
                    case TYPE::FUNCTION: \
                        throw err::common("Function variable is not convertible to String."); \
                    break; \
                } \
            break; \
            \
            case TYPE::NUMBER: \
                switch(rhs.type) { \
                    case TYPE::INTEGER: \
                        return var{ \
                            integer{std::get<number>(value) op static_cast<number>(std::get<integer>(rhs.value))} \
                        }; \
                    break; \
                    \
                    case TYPE::NUMBER: \
                        return var{ \
                            integer{std::get<number>(value) op std::get<number>(rhs.value)} \
                        }; \
                    break; \
                    \
                    case TYPE::NIHIL: \
                        throw err::common("Nihil variable is not convertible to Number."); \
                    break; \
                    \
                    case TYPE::STRING: \
                        throw err::common("String variable is not convertible to Number."); \
                    break; \
                    \
                    case TYPE::OBJECT: \
                        throw err::common("Object variable is not convertible to Number."); \
                    break; \
                    \
                    case TYPE::FUNCTION: \
                        throw err::common("Function variable is not convertible to String."); \
                    break; \
                } \
            break; \
            \
            case TYPE::STRING: \
                switch(rhs.type) { \
                    case TYPE::INTEGER: \
                        throw err::common("Integer variable is not convertible to String."); \
                    break; \
                    \
                    case TYPE::NUMBER: \
                        throw err::common("Number variable is not convertible to String."); \
                    break; \
                    \
                    case TYPE::NIHIL: \
                        throw err::common("Nihil variable is not convertible to String."); \
                    break; \
                    \
                    case TYPE::STRING: \
                        return var{ \
                            integer{std::get<string>(value) op std::get<string>(rhs.value)} \
                        }; \
                    break; \
                    \
                    case TYPE::OBJECT: \
                        throw err::common("Object variable is not convertible to String."); \
                    break; \
                    \
                    case TYPE::FUNCTION: \
                        throw err::common("Function variable is not convertible to String."); \
                    break; \
                } \
            break; \
            \
            case TYPE::OBJECT: \
                throw err::common("Object variable can't be compared."); \
            break; \
            \
            case TYPE::FUNCTION: \
                throw err::common("Function variable can't be compared."); \
            break; \
        } \
        \
        throw err::common("Poisoned variable."); \
    } \

LOGICAL_OP(>);
LOGICAL_OP(<);
LOGICAL_OP(<=);
LOGICAL_OP(>=);
LOGICAL_OP(==);
LOGICAL_OP(!=);

#undef LOGICAL_OP
