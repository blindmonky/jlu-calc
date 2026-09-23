#ifndef STACK_HPP
#define STACK_HPP

template <typename T, int size = 100>
class Stack
{
public:
    Stack();

    void push(const T &val);
    bool empty() const;
    bool full() const;
    void print() const;
    T pop();
    const T &top() const;

private:
    T data[size]; // Array of stack elements
    int top_idx;  // Index of the top of the stack
};

#endif

#include <iostream>
#include <stdexcept>

template <typename T, int size>
Stack<T, size>::Stack() : top_idx(-1) {}

template <typename T, int size>
void Stack<T, size>::push(const T &val)
{
    if (full())
    {
        throw std::out_of_range("Full stack");
    }
    else
    {
        data[++top_idx] = val;
    }
}

template <typename T, int size>
bool Stack<T, size>::empty() const
{
    return top_idx == -1;
}

template <typename T, int size>
bool Stack<T, size>::full() const
{
    return top_idx == size - 1;
}

template <typename T, int size>
void Stack<T, size>::print() const
{
    std::cout << "[ ";
    int i = top_idx;
    while (i >= 0)
    {
        std::cout << data[i];
        if (i > 0)
        {
            std::cout << ", ";
        }
        i--;
    }
    std::cout << " ]\n";
}

template <typename T, int size>
T Stack<T, size>::pop()
{
    if (empty())
    {
        throw std::out_of_range("Empty stack");
    }
    else
    {
        return data[top_idx--];
    }
}

template <typename T, int size>
const T &Stack<T, size>::top() const
{
    if (empty())
    {
        throw std::out_of_range("Empty stack");
    }
    else
    {
        return data[top_idx];
    }
}
