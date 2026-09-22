#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "Stack.hpp"

#include <string>
#include <vector>

class Calculator
{
public:
    Calculator(const std::string &infix = "");

    std::string toPostfix(); // Throws std::logic_error
    double calculate() const;

private:
    std::string infix;
    std::vector<std::string> postfix; // Postfix tokens, so multi-digit operands survive

    // Converts the stored infix expression into postfix tokens.
    // Throws std::logic_error if the expression is malformed.
    std::vector<std::string> buildPostfix() const;

    static bool isOperator(char c);
    static bool isOpenBracket(char c);
    static bool isCloseBracket(char c);
    static bool bracketsMatch(char open, char close);
    static int precedence(char op);
    static bool isRightAssociative(char op);
    static double applyOperator(double lhs, double rhs, char op);
};

#endif
