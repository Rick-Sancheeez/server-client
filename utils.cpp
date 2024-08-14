// utils.cpp
#include "utils.h"
#include <sstream>
#include <stdexcept>
int Math(const std::string& expression) {
    int result = 0;
    char op;
    int num;
    std::stringstream ss(expression);

    ss >> result; // Читаємо перше число

    while (ss >> op >> num) {
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': 
                if (num != 0) result /= num; 
                else throw std::invalid_argument("Division by zero");
                break;
            default: throw std::invalid_argument("Invalid operator");
        }
    }

    return result;
}