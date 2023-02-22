#include "lexer.hh"

void fell::lex::solve_table_member(types::variable::var & tbl, std::string_view def) {
    if(def[0] != '"')
        throw std::runtime_error{"Table key should be a String."};

    std::size_t i = 1;

    while(i < def.length()) {
        if(def[i] == '"' && def[i] != '\\')
            break;
        ++i;
    }

    if(i == def.length())
        throw std::runtime_error{"Unterminated string."};

    const auto name = def.substr(1, i - 1);
    ++i;

    while(i < def.length() && def[i] != ':') {
        if(!std::isspace(def[i]))
            throw std::runtime_error{"Extra token in definition."};

        ++i;
    }

    if(i == def.length())
        throw std::runtime_error{"Expected property for key: " + std::string(name)};

    const auto expr = std::string_view{def.begin() + static_cast<std::int64_t>(i) + 1, def.end()};

    if(expr.begin() == expr.end())
        throw std::runtime_error{"Expected property."};

    (*tbl)[std::string(name)] = solve_expression(expr);
}

void fell::lex::solve_variable(const std::string_view & expr, std::stack<types::variable::var> & vars, std::size_t & i, bool & alternance) {
    if(alternance == true)
        throw std::runtime_error{"Extra token."};
    alternance = true;

    if(expr[i] == '"') {
        ++i;

        const std::size_t j = i;

        while(i < expr.length()) {
            if(expr[i] == '"' && expr[i] != '\\')
                break;
            ++i;
        }

        if(i == expr.length())
            throw std::runtime_error{"Unterminated string."};

        vars.push(util::make_var<types::string>(std::string{expr.data() + j, i - j}));
    } else if(expr[i] == '{') {
        ++i;

        auto tbl = util::make_var<types::table>();

        std::size_t paren_counter = 1;

        std::size_t j = i;

        bool triggered = false;

        while(paren_counter) {
            if(expr[i] == '{')
                ++paren_counter;
            else if(expr[i] == ':' && paren_counter == 1)
                triggered = false;
            else if(expr[i] == '}') {
                --paren_counter;
                if(paren_counter == 0 && !triggered) {
                    while(std::isspace(expr[j])) ++j;
                    const auto copy = i;
                    --i;
                    while(std::isspace(expr[i])) --i;
                    solve_table_member(tbl, std::string_view{expr.data() + j, i - j + 1});
                    i = copy;
                }
            } else if(expr[i] == ',' && paren_counter == 1) {
                while(std::isspace(expr[j])) ++j;
                const auto copy = i;
                --i;
                while(std::isspace(expr[i])) --i;
                solve_table_member(tbl, std::string_view{expr.data() + j, i - j + 1});
                triggered = true;
                i = copy;
                j = i + 1;
            }
            ++i;
        }

        vars.push(std::move(tbl));
    } else {
        const std::size_t j = i;

        while(std::strchr(" +-%*/)", expr[i]) == 0 && i < expr.length()) {
            ++i;
        }
        const auto var = std::string{expr.data() + j, i - j};
        --i;

        types::variable::var intermediary;

        try {
            intermediary = check_for_constant_expression(var);
        } catch(...) {
            const auto & ref = (*lang::global_table)[var];

            if(ref == nullptr)
                throw std::runtime_error{"Undefined variable: " + var};

            util::copy(intermediary, ref);
        }

        vars.push(std::move(intermediary));
    }
}

fell::types::variable::var fell::lex::apply_operation(
    const types::variable::var && lhs,
    const types::variable::var && rhs,
    const std::string && operation
) {
    if(operation == "+")
        return *lhs + rhs;

    if(operation == "-")
        return *lhs - rhs;

    if(operation == "%")
        return *lhs % rhs;

    if(operation == "*")
        return *lhs * rhs;

    if(operation == "/")
        return *lhs / rhs;

    throw std::runtime_error{"Unknown operator: " + operation};
}

std::size_t fell::lex::operator_precedence(const std::string & operation) {
    if(operation == "*" || operation == "/")
        return 2;
    if(operation == "+" || operation == "-" || operation == "%")
        return 1;
    if(operation == "(")
        return 0;

    throw std::runtime_error{"Unknown operator: " + operation};
}

fell::types::variable::var fell::lex::check_for_constant_expression(const std::string & expr) {
    if(expr == keywords::FALSE)
        return util::make_var<fell::types::number>(0);
    else if(expr == keywords::TRUE)
        return util::make_var<fell::types::number>(1);
    else if(expr == keywords::NIHIL)
        return util::make_var<fell::types::nihil>();

    return util::make_var<fell::types::number>(std::stod(expr));
}

fell::types::variable::var fell::lex::solve_expression(const std::string_view expr) {
    std::stack<types::variable::var> vars;
    std::stack<std::string> operators;

    const auto solve_stacks = [&]() {
        const auto operation = std::move(operators.top());
        operators.pop();

        if(vars.empty())
            throw std::runtime_error{"Extra symbol: " + operation};

        const auto rhs = std::move(vars.top());
        vars.pop();

        if(vars.empty())
            throw std::runtime_error{"Extra symbol: " + operation};

        const auto lhs = std::move(vars.top());
        vars.pop();

        vars.push(apply_operation(std::move(lhs), std::move(rhs), std::move(operation)));
    };

    bool alternance = false;

    for(std::size_t i = 0; i < expr.length(); ++i) {
        if(std::isspace(expr[i])) {
            continue;
        } else if(expr[i] == '(') {
            operators.push("(");
        } else if(std::strchr("+-%*/)", expr[i]) == 0) {
            solve_variable(expr, vars, i, alternance);
        } else if(expr[i] == ')') {
            while(!operators.empty() && operators.top() != "(")
                solve_stacks();

            if(!operators.empty())
                operators.pop();
        } else {
            if(alternance == false)
                throw std::runtime_error{"Extra symbol."};
            alternance = false;

            std::string next_operation{}; next_operation.reserve(2);

            while(std::strchr("+-%*/", expr[i]) && i < expr.length()) {
                next_operation.push_back(expr[i]);
                ++i;
            }

            --i;

            while(!operators.empty() && (operator_precedence(operators.top()) >= operator_precedence(next_operation)))
                solve_stacks();

            operators.push(next_operation);
        }
    }

    while(!operators.empty())
        solve_stacks();

    return std::move(vars.top());
}
