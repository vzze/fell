#include "lexer.hh"

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
    if(operation == "+" || operation == "-" || operation == "%")
        return 1;
    if(operation == "*" || operation == "/")
        return 2;

    throw std::runtime_error{"Unknown operator: " + operation};
}

fell::types::variable::var fell::lex::check_for_constant_expression(const std::string expr) {
    if(expr == keywords::FALSE)
        return util::make_var<fell::types::number>(0);
    else if(expr == keywords::TRUE)
        return util::make_var<fell::types::number>(1);
    else if(expr == keywords::NIHIL)
        return util::make_var<fell::types::nihil>();

    return util::make_var<fell::types::number>(std::stod(expr));
}

fell::types::variable::var fell::lex::solve_expression(const std::string && expr) {
    std::stack<types::variable::var> vars;
    std::stack<std::string> operators;

    for(std::size_t i = 0; i < expr.length(); ++i) {
        if(std::isspace(expr[i]))
            continue;
        else if(expr[i] == '(') {
            operators.push("(");
        } else if(std::strchr("+-%*/", expr[i]) == 0) {
            std::string var{""};

            while(std::strchr("+-%*/", expr[i]) == 0) {
                var.push_back(expr[i]);
                ++i;
            }

            --i;
            var = util::trim(var);

            if(var.find(' ') != std::string::npos)
                throw std::runtime_error("Extra token.");

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
        } else if(expr[i] == ')') {
            while(!operators.empty() && operators.top() != "(") {
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
            }

            if(!operators.empty())
                operators.pop();
        } else {
            std::string next_operation = "";

            while(std::strchr("+-%*/", expr[i])) {
                next_operation.push_back(expr[i]);
                ++i;
            }

            --i;

            while(!operators.empty() && (operator_precedence(operators.top()) >= operator_precedence(next_operation))) {
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
            }

            operators.push(next_operation);
        }
    }

    while(!operators.empty()) {
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
    }

    return std::move(vars.top());
}
