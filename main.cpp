#include "Calculator.hpp"

#include <iostream>
#include <stdexcept>
#include <iomanip>

int main(void)
{
    std::string infixExpression;
    std::cout << "Enter an infix expression: ";
    getline(std::cin, infixExpression);

    Calculator calc(infixExpression);

    try
    {
        std::string postfix = calc.toPostfix();
        std::cout << "Postfix expression: " << postfix << std::endl;
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Result: " << calc.calculate() << std::endl;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}Ï
