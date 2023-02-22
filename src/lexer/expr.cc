#include "lexer.hh"

void fell::lex::solve_expression_stacks(std::stack<types::variable::var> & vars, std::stack<std::string_view> & operators) {
    const auto operation = std::move(operators.top());
    operators.pop();

    if(vars.empty())
        throw std::runtime_error{"Extra symbol: " + std::string{operation}};

    const auto rhs = std::move(vars.top());
    vars.pop();

    if(vars.empty())
        throw std::runtime_error{"Extra symbol: " + std::string{operation}};

    const auto lhs = std::move(vars.top());
    vars.pop();

    vars.push(apply_operation(std::move(lhs), std::move(rhs), operation));
}

fell::types::variable::var fell::lex::apply_operation(
    const types::variable::var && lhs,
    const types::variable::var && rhs,
    const std::string_view operation
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

    throw std::runtime_error{"Unknown operator: " + std::string{operation}};
}

std::size_t fell::lex::operator_precedence(const std::string_view operation) {
    if(operation == "*" || operation == "/")
        return 2;
    if(operation == "+" || operation == "-" || operation == "%")
        return 1;
    if(operation == "(")
        return 0;

    throw std::runtime_error{"Unknown operator: " + std::string{operation}};
}

fell::types::variable::var fell::lex::solve_expression(const std::string_view expr) {
    std::stack<types::variable::var> vars;
    std::stack<std::string_view> operators;

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

            std::size_t j = i;

            while(std::strchr("+-%*/", expr[i]) && i < expr.length())
                ++i;

            const std::string_view next_operation{expr.data() + j, i - j};

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
