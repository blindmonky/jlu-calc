#include "Calculator.hpp"

#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    const char *const BAD_COUNT =
        "Error: Invalid number of operands/operators in the expression";
    const char *const IMBALANCED =
        "Error: Imbalanced parentheses in the expression";
}

Calculator::Calculator(const std::string &infix) : infix(infix) {}

bool Calculator::isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

bool Calculator::isOpenBracket(char c)
{
    return c == '(' || c == '{' || c == '[';
}

bool Calculator::isCloseBracket(char c)
{
    return c == ')' || c == '}' || c == ']';
}

bool Calculator::bracketsMatch(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

int Calculator::precedence(char op)
{
    if (op == '+' || op == '-')
    {
        return 1;
    }
    else if (op == '*' || op == '/' || op == '%')
    {
        return 2;
    }
    else if (op == '^')
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

bool Calculator::isRightAssociative(char op)
{
    return op == '^';
}

double Calculator::applyOperator(double lhs, double rhs, char op)
{
    if (op == '+')
    {
        return lhs + rhs;
    }
    else if (op == '-')
    {
        return lhs - rhs;
    }
    else if (op == '*')
    {
        return lhs * rhs;
    }
    else if (op == '/')
    {
        if (rhs == 0.0)
        {
            throw std::logic_error("Error: Division by zero");
        }
        return lhs / rhs;
    }
    else if (op == '%')
    {
        if (rhs == 0.0)
        {
            throw std::logic_error("Error: Modulo by zero");
        }
        return std::fmod(lhs, rhs);
    }
    else if (op == '^')
    {
        return std::pow(lhs, rhs);
    }
    else
    {
        throw std::logic_error("Error: Unknown operator in the expression");
    }
}

// Shunting-yard conversion. expectOperand tracks what may legally come next,
// which is what catches "1+", "+1", "2 3" and "(2+)".
std::vector<std::string> Calculator::buildPostfix() const
{
    std::vector<std::string> output;
    Stack<char> operators;
    bool expectOperand = true;

    std::string::size_type i = 0;
    while (i < infix.size())
    {
        const char c = infix[i];

        if (std::isspace(static_cast<unsigned char>(c)))
        {
            ++i;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            if (!expectOperand)
            {
                throw std::logic_error(BAD_COUNT);
            }
            std::string number;
            while (i < infix.size() &&
                   std::isdigit(static_cast<unsigned char>(infix[i])))
            {
                number += infix[i];
                ++i;
            }
            output.push_back(number);
            expectOperand = false;
            continue;
        }

        if (isOperator(c))
        {
            if (expectOperand)
            {
                throw std::logic_error(BAD_COUNT);
            }
            while (!operators.empty() && isOperator(operators.top()) &&
                   (precedence(operators.top()) > precedence(c) ||
                    (precedence(operators.top()) == precedence(c) &&
                     !isRightAssociative(c))))
            {
                output.push_back(std::string(1, operators.pop()));
            }
            operators.push(c);
            expectOperand = true;
            ++i;
            continue;
        }

        if (isOpenBracket(c))
        {
            if (!expectOperand)
            {
                throw std::logic_error(BAD_COUNT);
            }
            operators.push(c);
            ++i;
            continue;
        }

        if (isCloseBracket(c))
        {
            if (expectOperand)
            {
                throw std::logic_error(BAD_COUNT);
            }
            bool matched = false;
            while (!operators.empty())
            {
                const char symbol = operators.pop();
                if (isOpenBracket(symbol))
                {
                    if (!bracketsMatch(symbol, c))
                    {
                        throw std::logic_error(
                            "Error: Mismatched brackets in the expression");
                    }
                    matched = true;
                    break;
                }
                output.push_back(std::string(1, symbol));
            }
            if (!matched)
            {
                throw std::logic_error(IMBALANCED);
            }
            ++i;
            continue;
        }

        throw std::logic_error(
            std::string("Error: Invalid character in the expression: '") + c + "'");
    }

    if (expectOperand)
    {
        throw std::logic_error(BAD_COUNT);
    }

    while (!operators.empty())
    {
        const char symbol = operators.pop();
        if (isOpenBracket(symbol))
        {
            throw std::logic_error(IMBALANCED);
        }
        output.push_back(std::string(1, symbol));
    }

    return output;
}

std::string Calculator::toPostfix()
{
    postfix = buildPostfix();

    std::string result;
    for (std::vector<std::string>::size_type i = 0; i < postfix.size(); ++i)
    {
        result += postfix[i];
    }
    return result;
}

double Calculator::calculate() const
{
    // Reuse the tokens from toPostfix() when they exist, so calculate() still
    // works if it is called on its own.
    const std::vector<std::string> tokens =
        postfix.empty() ? buildPostfix() : postfix;

    Stack<double> operands;

    for (std::vector<std::string>::size_type i = 0; i < tokens.size(); ++i)
    {
        const std::string &token = tokens[i];

        if (token.size() == 1 && isOperator(token[0]))
        {
            if (operands.empty())
            {
                throw std::logic_error(BAD_COUNT);
            }
            const double rhs = operands.pop();

            if (operands.empty())
            {
                throw std::logic_error(BAD_COUNT);
            }
            const double lhs = operands.pop();

            operands.push(applyOperator(lhs, rhs, token[0]));
        }
        else
        {
            operands.push(std::stod(token));
        }
    }

    if (operands.empty())
    {
        throw std::logic_error(BAD_COUNT);
    }

    const double result = operands.pop();

    if (!operands.empty())
    {
        throw std::logic_error(BAD_COUNT);
    }

    return result;
}
