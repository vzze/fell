#include "lexer.hh"

void fell::lex::solve_expression_stacks(std::stack<types::variable::var> & vars, std::stack<std::string> & operators) {
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

fell::types::variable::var fell::lex::solve_expression(const std::string_view expr) {
    std::stack<types::variable::var> vars;
    std::stack<std::string> operators;

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
                solve_expression_stacks(vars, operators);

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
                solve_expression_stacks(vars, operators);

            operators.push(next_operation);
        }
    }

    while(!operators.empty())
        solve_expression_stacks(vars, operators);

    return std::move(vars.top());
}
