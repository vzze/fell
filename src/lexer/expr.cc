#include "lexer.hh"

void fell::lex::solve_variable(const std::string_view & expr, std::stack<types::variable::var> & vars, std::size_t & i) {
    std::string var{""};

    while(std::strchr("+-%*/)", expr[i]) == 0) {
        if(expr[i] == '"') {
            ++i;

            while(i < expr.length()) {
                if(expr[i] == '"' && expr[i] != '\\')
                    break;
                var.push_back(expr[i]);
                ++i;
            }

            if(i == expr.length())
                throw std::runtime_error{"Unterminated string."};

            vars.push(util::make_var<types::string>(var));

            return;
        }

        var.push_back(expr[i]);
        ++i;
    }

    --i;
    util::trim(var);

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

    static const auto solve_stacks = [&]() {
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

    for(std::size_t i = 0; i < expr.length(); ++i) {
        if(std::isspace(expr[i])) {
            continue;
        } else if(expr[i] == '(') {
            operators.push("(");
        } else if(std::strchr("+-%*/)", expr[i]) == 0) {
            solve_variable(expr, vars, i);
        } else if(expr[i] == ')') {
            while(!operators.empty() && operators.top() != "(")
                solve_stacks();

            if(!operators.empty())
                operators.pop();
        } else {
            std::string next_operation = "";

            while(std::strchr("+-%*/", expr[i])) {
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
