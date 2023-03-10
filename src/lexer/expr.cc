#include "lexer.hh"

void fell::lex::solve_expression_stacks(
    std::stack<inmemory> & vars,
    std::queue<inmemory> & par_list,
    std::stack<std::string_view> & operators
) {
    const auto operation = std::move(operators.top());
    operators.pop();

    if(operation == ",") {
        if(vars.empty())
            throw std::runtime_error{"Extra symbol: " + std::string{operation}};

        par_list.push(std::move(vars.top()));
        vars.pop();
    } else {
        if(vars.empty())
            throw std::runtime_error{"Extra symbol: " + std::string{operation}};

        const auto rhs = std::move(vars.top());
        vars.pop();

        if(vars.empty())
            throw std::runtime_error{"Extra symbol: " + std::string{operation}};

        const auto lhs = std::move(vars.top());
        vars.pop();

        apply_operation(std::move(lhs), std::move(rhs), operation, vars);
    }
}

#define SOLVE_DEFAULT_VARS(symbol, op) \
    if(operation == #symbol) { \
        if(lhs.non_reference) { \
            if(rhs.non_reference) { \
                vars.emplace(*lhs.non_reference op rhs.non_reference.get()); return; \
            } else { \
                vars.emplace(*lhs.non_reference op rhs.reference->get()); return; \
            } \
        } else { \
            if(rhs.non_reference) { \
                vars.emplace(**lhs.reference op rhs.non_reference.get()); return; \
            } else { \
                vars.emplace(**lhs.reference op rhs.reference->get()); return; \
            } \
        } \
    }

void fell::lex::apply_operation(
    const inmemory && lhs,
    const inmemory && rhs,
    const std::string_view operation,
    std::stack<inmemory> & vars
) {

    SOLVE_DEFAULT_VARS(*, *);
    SOLVE_DEFAULT_VARS(/, /);

    SOLVE_DEFAULT_VARS(+, +);
    SOLVE_DEFAULT_VARS(-, -);
    SOLVE_DEFAULT_VARS(%, %);

    SOLVE_DEFAULT_VARS(<, <);
    SOLVE_DEFAULT_VARS(<=, <=)
    SOLVE_DEFAULT_VARS(>, >);
    SOLVE_DEFAULT_VARS(>=, >=);

    SOLVE_DEFAULT_VARS(==, ==);
    SOLVE_DEFAULT_VARS(!=, !=);

    SOLVE_DEFAULT_VARS(&&, &&);
    SOLVE_DEFAULT_VARS(?, ||);

#undef SOLVE_DEFAULT_VARS

    if(operation == "=") {
        if(lhs.non_reference) {
            throw std::runtime_error{"Attempt to bind value to temporary variable."};
        } else {
            if(rhs.non_reference) {
                util::copy(*lhs.reference, rhs.non_reference);
            } else {
                util::copy(*lhs.reference, *rhs.reference);
            }
        }

        vars.emplace(util::make_var<types::nihil>());
        return;
    }

    if(operation == "[") {
        if(lhs.non_reference) {
            if(rhs.non_reference) {
                auto & s = (*lhs.non_reference)[rhs.non_reference.get()];

                if(s == nullptr)
                    s = util::make_var<types::nihil>();

                vars.emplace(&s);
                return;
            } else {
                auto & s = (*lhs.non_reference)[rhs.reference->get()];

                if(s == nullptr)
                    s = util::make_var<types::nihil>();

                vars.emplace(&s);
                return;
            }
        } else {
            if(rhs.non_reference) {
                auto & s = (**lhs.reference)[rhs.non_reference.get()];

                if(s == nullptr)
                    s = util::make_var<types::nihil>();

                vars.emplace(&s);
                return;
            } else {
                auto & s = (**lhs.reference)[rhs.reference->get()];

                if(s == nullptr)
                    s = util::make_var<types::nihil>();

                vars.emplace(&s);
                return;
            }
        }
    }

    throw std::runtime_error{"Unknown operator: " + std::string{operation}};
}

std::size_t fell::lex::operator_precedence(const std::string_view operation) {
    if(operation == "*" || operation == "/")
        return 8;
    if(operation == "+" || operation == "-" || operation == "%")
        return 7;
    if(operation == "<" || operation == "<=" || operation == ">" || operation == ">=")
        return 6;
    if(operation == "==" || operation == "!=")
        return 5;
    if(operation == "&&")
        return 4;
    if(operation == "?")
        return 3;
    if(operation == "=")
        return 2;
    if(operation == ",")
        return 1;
    if(operation == "(" || operation == "[")
        return 0;

    throw std::runtime_error{"Unknown operator: " + std::string{operation}};
}

std::size_t fell::lex::solve_expression(
    const std::string_view expr,
    types::variable::var * v,
    std::string_view op
) {
    std::stack<inmemory> vars;
    std::list<std::queue<inmemory>> parameter_list;
    std::stack<std::string_view> operators;

    if(v != nullptr) {
        vars.emplace(v);
        operators.push(op);
    }

    bool alternance = false;
    std::list<bool> func_call = {};
    std::list<bool> func_no_param = {};
    std::size_t i;

    for(i = 0; i < expr.length() && expr[i] != ';'; ++i) {
        if(std::isspace(expr[i])) {
            continue;
        } else if(expr[i] == '(') {
            operators.push("(");
            if(alternance == true) {
                func_call.push_back(true);
                auto j = i + 1;
                while(std::isspace(expr[j])) ++j;
                if(expr[j] == ')') func_no_param.push_back(true);
                else func_no_param.push_back(false);
                alternance = false;
                parameter_list.emplace_back();
            }
        } else if(expr[i] == '[') {
            operators.push("[");
            alternance = false;
        } else if(expr[i] == ']') {
            while(!operators.empty() && operators.top() != "[")
                solve_expression_stacks(vars, *parameter_list.rbegin(), operators);

            if(!operators.empty()) {
                solve_expression_stacks(vars, *parameter_list.rbegin(), operators);
            }
        } else if(expr[i] == ')') {
            while(!operators.empty() && operators.top() != "(")
                solve_expression_stacks(vars, *parameter_list.rbegin(), operators);

            if(!operators.empty()) {
                operators.pop();

                if(func_call.rbegin() != func_call.rend()) {
                    alternance = true;

                    func_call.pop_back();

                    std::vector<inmemory> params;

                    params.reserve(parameter_list.rbegin()->size() + 1);

                    if(!*func_no_param.rbegin()) {
                        params.push_back(std::move(vars.top()));
                        vars.pop();
                    }

                    func_no_param.pop_back();

                    while(!parameter_list.rbegin()->empty()) {
                        params.push_back(std::move(parameter_list.rbegin()->front()));
                        parameter_list.rbegin()->pop();
                    }

                    parameter_list.pop_back();

                    const auto lhs = std::move(vars.top());
                    vars.pop();

                    if(lhs.reference) {
                        vars.emplace((*lhs.reference)->call(std::move(params)));
                    } else {
                        vars.emplace(lhs.non_reference->call(std::move(params)));
                    }
                }
            }
        } else if(std::strchr(",=<>!&?+-%*/", expr[i]) == 0) {
            solve_variable(expr, vars, i, alternance);
        } else {
            if(alternance == false)
                throw std::runtime_error{"Extra symbol."};
            alternance = false;

            std::size_t j = i;

            while(std::strchr(",=<>!&?+-%*/", expr[i]) && i < expr.length())
                ++i;

            const std::string_view next_operation{expr.data() + j, i - j};

            --i;

            while(!operators.empty() && (operator_precedence(operators.top()) >= operator_precedence(next_operation)))
                solve_expression_stacks(vars, *parameter_list.rbegin(), operators);

            operators.push(next_operation);
        }
    }

    while(!operators.empty())
        solve_expression_stacks(vars, *parameter_list.rbegin(), operators);

    return i + 1;
}
