#include <main.hpp>

int main() {
    
    while (true) {

        std::string expression_string;

        std::getline(std::cin, expression_string);

        char* expression = new char[expression_string.length() + 1];
        std::strcpy(expression, expression_string.c_str());

        std::vector<InfixToken>* infix_tokens;

        infix_tokens = read_expression(expression);
    
        std::vector<PostfixToken>* postfix_tokens;

        postfix_tokens = convert_expression(infix_tokens);

        double result = evaluate_expression(postfix_tokens);

        std::cout << result;
        std::cout << std::endl;
    }

    return 0;
}

std::vector<InfixToken>* read_expression(char* expression) {

    std::vector<InfixToken>* infix_tokens = new std::vector<InfixToken>();
    InfixTokenType state = InfixTokenType::Unknown;
    
    int i = 0;
    std::vector<char>* token_string_buffer = new std::vector<char>;

    auto push_token = [&]() {

        std::string token_string(token_string_buffer->begin(), token_string_buffer->end());
        InfixToken* token;

        if (state == InfixTokenType::Float || state == InfixTokenType::Integer) {
            double number = std::stof(token_string);
            token = new InfixToken(state, number);
        }
        else if (state == InfixTokenType::LeftParenthesis || state == InfixTokenType::RightParenthesis) {

            token = new InfixToken(state);
        }
        else {
            if (token_string == "*") token = new InfixToken(state, OperatorType::Multiply);
            else if (token_string == "+") token = new InfixToken(state, OperatorType::Add);
            else if (token_string == "-") token = new InfixToken(state, OperatorType::Subtract);
            else if (token_string == "/") token = new InfixToken(state, OperatorType::Divide);
            else if (token_string == "^") token = new InfixToken(state, OperatorType::Raise);

            if (state == InfixTokenType::Prefix && token_string != "-") throw std::runtime_error("An error occurred when parsing the expression");
        }

        infix_tokens->push_back(*token);

        token_string_buffer = new std::vector<char>;
    };

    while (true)
    {
        char c = expression[i];
        if (c == ' ') {
            i++;
            continue;
        }
        if (c == '\0') {

            if (i == 0) throw std::runtime_error("An error occurred when parsing the expression");
            push_token();
            break;
        }

        switch (state) {

            case InfixTokenType::Unknown:

                if (isdigit(c)) {
                    state = InfixTokenType::Integer;
                }
                else if (isoper(c)) {
                    state = InfixTokenType::Prefix;
                }
                else if (c == '(') {
                    state = InfixTokenType::LeftParenthesis;
                }
                else throw std::runtime_error("An error occurred when parsing the expression");

                break;

            case InfixTokenType::Integer:

                if (isdigit(c)) {
                    token_string_buffer->push_back(c);
                    i++;
                }
                else if (c == '.') {
                    token_string_buffer->push_back(c);
                    i++;
                    state = InfixTokenType::Float;
                }
                else if (isoper(c)) {
                    push_token();
                    state = InfixTokenType::Infix;
                }
                else if (c == ')') {
                    push_token();
                    state = InfixTokenType::RightParenthesis;
                    i++;
                }
                else throw std::runtime_error("An error occurred when parsing the expression");

                break;

            case InfixTokenType::Float:

                if (isdigit(c)) {
                    token_string_buffer->push_back(c);
                    i++;
                }
                else if (isoper(c)) {
                    push_token();
                    state = InfixTokenType::Infix;
                }
                else throw std::runtime_error("An error occurred when parsing the expression");

                break;

            case InfixTokenType::Infix:

                if (token_string_buffer->size() == 0) {
                    token_string_buffer->push_back(c);
                    i++;
                }
                else {
                    push_token();
                    if (isdigit(c)) {
                        state = InfixTokenType::Integer;
                    }
                    else if (isoper(c)) {
                        state = InfixTokenType::Prefix;
                    }
                    else if (c == '(') {
                        state = InfixTokenType::LeftParenthesis;
                    }
                    else throw std::runtime_error("An error occurred when parsing the expression");
                }

                break;

            case InfixTokenType::LeftParenthesis:

                if (token_string_buffer->size() == 0) {
                    token_string_buffer->push_back(c);
                    i++;
                }
                else {
                    push_token();
                    if (isdigit(c)) {
                        state = InfixTokenType::Integer;
                    }
                    else if (isoper(c)) {
                        state = InfixTokenType::Prefix;
                    }
                    else throw std::runtime_error("An error occurred when parsing the expression");
                }

                break;

            case InfixTokenType::RightParenthesis:

                if (isoper(c)) {
                    push_token();
                    state = InfixTokenType::Infix;
                }
                else if (c == ')') {
                    push_token();
                    state = InfixTokenType::RightParenthesis;
                }
                else throw std::runtime_error("An error occurred when parsing the expression");
                break;

            case InfixTokenType::Prefix:

                if (token_string_buffer->size() == 0) {
                    token_string_buffer->push_back(c);
                    i++;
                }
                else {
                    push_token();
                    if (isdigit(c)) {
                        state = InfixTokenType::Integer;
                    }
                    else if (isoper(c)) {
                        state = InfixTokenType::Prefix;
                    }
                    else throw std::runtime_error("An error occurred when parsing the expression");
                }

                break;
        }
    }

    return infix_tokens;
}
std::vector<PostfixToken>* convert_expression(std::vector<InfixToken>* expression) {

    std::map<OperatorType, int> precedence{
        {OperatorType::Add, 1},
        {OperatorType::Subtract, 1},
        {OperatorType::Multiply, 2},
        {OperatorType::Divide, 2},
        {OperatorType::Raise, 3},
    };
    
    std::vector<PostfixToken>* postfix_tokens = new std::vector<PostfixToken>;

    std::stack<InfixToken>* operator_stack = new std::stack<InfixToken>;

    for (int i = 0; i < expression->size(); i++) {
        InfixToken infix_token = (*expression)[i];

        if (infix_token.type == InfixTokenType::Integer || infix_token.type == InfixTokenType::Float) {

            PostfixToken postfix_token = PostfixToken(PostfixTokenType::Number, infix_token.number);
            postfix_tokens->push_back(postfix_token);

            while (operator_stack->size() != 0 && operator_stack->top().type == InfixTokenType::Prefix) {

                InfixToken topmost_operator = operator_stack->top();
                operator_stack->pop();
                PostfixToken postfix_token = PostfixToken(PostfixTokenType::UnaryOp, topmost_operator.oper);
                postfix_tokens->push_back(postfix_token);
            }
        }
        else if (infix_token.type == InfixTokenType::LeftParenthesis) {

            operator_stack->push(infix_token);
        }
        else if (infix_token.type == InfixTokenType::RightParenthesis) {

            while (operator_stack->size() != 0 && operator_stack->top().type != InfixTokenType::LeftParenthesis && precedence[operator_stack->top().oper] >= precedence[infix_token.oper]) {

                InfixToken topmost_operator = operator_stack->top();
                operator_stack->pop();
                PostfixToken postfix_token = PostfixToken(PostfixTokenType::BinaryOp, topmost_operator.oper);
                postfix_tokens->push_back(postfix_token);
            }

            operator_stack->pop();
        }
        else if (infix_token.type == InfixTokenType::Infix) {

            while (operator_stack->size() != 0 && operator_stack->top().type != InfixTokenType::LeftParenthesis && precedence[operator_stack->top().oper] >= precedence[infix_token.oper]) {

                InfixToken topmost_operator = operator_stack->top();
                operator_stack->pop();
                PostfixToken postfix_token = PostfixToken(PostfixTokenType::BinaryOp, topmost_operator.oper);
                postfix_tokens->push_back(postfix_token);
            }
            operator_stack->push(infix_token);
        }
        else if (infix_token.type == InfixTokenType::Prefix) {

            operator_stack->push(infix_token);
        }
    }
    while (operator_stack->size() != 0) {

        InfixToken topmost_operator = operator_stack->top();
        operator_stack->pop();
        PostfixToken postfix_token = PostfixToken(PostfixTokenType::BinaryOp, topmost_operator.oper);
        postfix_tokens->push_back(postfix_token);
    }

    return postfix_tokens;
}
double evaluate_expression(std::vector<PostfixToken>* expression) {
    
    std::stack<double> stack;

    for (int i = 0; i < expression->size(); i++) {
        PostfixToken token = (*expression)[i];

        if (token.type == PostfixTokenType::Number) {
            stack.push(token.number);
        }
        else if (token.type == PostfixTokenType::UnaryOp) {

            double a = stack.top(); stack.pop();

            switch (token.oper) {
                case OperatorType::Subtract: 
                    a = a * -1;
                    break;
            }

            stack.push(a);
        }
        else if (token.type == PostfixTokenType::BinaryOp) {

            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();

            double result;

            switch (token.oper) {
                case OperatorType::Add:
                    result = a + b;
                    break;
                case OperatorType::Subtract:
                    result = a - b;
                    break;
                case OperatorType::Multiply:
                    result = a * b;
                    break;
                case OperatorType::Divide:
                    result = a / b;
                    break;
                case OperatorType::Raise:
                    result = std::pow(a, b);
                    break;
            }

            stack.push(result);
        }
    }

    return stack.top();
}
bool isoper(char c) {
    return c == '*' || c == '/' || c == '-' || c == '+' || c == '^';
}