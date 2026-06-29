// NumericOverflows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>     // std::cout
#include <limits>       // std::numeric_limits
#include <optional>     // std::optional
#include <typeinfo>     // typeid

/// <summary>
/// Template function to abstract away the logic of:
///   start + (increment * steps)
///
/// This version protects against overflow by checking the maximum value
/// before each addition is performed.
/// </summary>
/// <typeparam name="T">A type with basic math functions</typeparam>
/// <param name="start">The number to start with</param>
/// <param name="increment">How much to add each step</param>
/// <param name="steps">The number of steps to iterate</param>
/// <returns>
/// A valid result when no overflow occurs.
/// std::nullopt when overflow would occur.
/// </returns>
template <typename T>
std::optional<T> add_numbers(T const& start, T const& increment, unsigned long int const& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // Detect overflow before performing the addition.
        // If result + increment would be greater than the maximum value
        // for type T, the operation is stopped and failure is returned.
        if (increment > 0 && result > std::numeric_limits<T>::max() - increment)
        {
            return std::nullopt;
        }

        result += increment;
    }

    return result;
}

/// <summary>
/// Template function to abstract away the logic of:
///   start - (decrement * steps)
///
/// This version protects against underflow by checking the lowest value
/// before each subtraction is performed.
/// </summary>
/// <typeparam name="T">A type with basic math functions</typeparam>
/// <param name="start">The number to start with</param>
/// <param name="decrement">How much to subtract each step</param>
/// <param name="steps">The number of steps to iterate</param>
/// <returns>
/// A valid result when no underflow occurs.
/// std::nullopt when underflow would occur.
/// </returns>
template <typename T>
std::optional<T> subtract_numbers(T const& start, T const& decrement, unsigned long int const& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // Detect underflow before performing the subtraction.
        // If result - decrement would be lower than the lowest value
        // for type T, the operation is stopped and failure is returned.
        //
        // lowest() is used instead of min() because for floating-point
        // types, min() is the smallest positive normalized value, not the
        // most negative value.
        if (decrement > 0 && result < std::numeric_limits<T>::lowest() + decrement)
        {
            return std::nullopt;
        }

        result -= decrement;
    }

    return result;
}


//  NOTE:
//    You will see the unary ('+') operator used in front of the variables in the test_XXX methods.
//    This forces the output to be a number for cases where cout would assume it is a character. 

template <typename T>
void test_overflow()
{
    // START DO NOT CHANGE
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we add each step (result should be: start + (increment * steps))
    const T increment = std::numeric_limits<T>::max() / steps;
    // whats our starting point
    const T start = 0;

    std::cout << "Overflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE

    std::cout << "\tAdding Numbers Without Overflow (" << +start << ", " << +increment << ", " << steps << ") = ";
    auto result = add_numbers<T>(start, increment, steps);

    if (result.has_value())
    {
        std::cout << +result.value() << " | overflow status: false" << std::endl;
    }
    else
    {
        std::cout << "Operation prevented | overflow status: true" << std::endl;
    }

    std::cout << "\tAdding Numbers With Overflow (" << +start << ", " << +increment << ", " << (steps + 1) << ") = ";
    result = add_numbers<T>(start, increment, steps + 1);

    if (result.has_value())
    {
        std::cout << +result.value() << " | overflow status: false" << std::endl;
    }
    else
    {
        std::cout << "Operation prevented | overflow status: true" << std::endl;
    }
}

template <typename T>
void test_underflow()
{
    // START DO NOT CHANGE
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we subtract each step (result should be: start - (increment * steps))
    const T decrement = std::numeric_limits<T>::max() / steps;
    // whats our starting point
    const T start = std::numeric_limits<T>::max();

    std::cout << "Underflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE

    std::cout << "\tSubtracting Numbers Without Overflow (" << +start << ", " << +decrement << ", " << steps << ") = ";
    auto result = subtract_numbers<T>(start, decrement, steps);

    if (result.has_value())
    {
        std::cout << +result.value() << " | underflow status: false" << std::endl;
    }
    else
    {
        std::cout << "Operation prevented | underflow status: true" << std::endl;
    }

    std::cout << "\tSubtracting Numbers With Overflow (" << +start << ", " << +decrement << ", " << (steps + 1) << ") = ";
    result = subtract_numbers<T>(start, decrement, steps + 1);

    if (result.has_value())
    {
        std::cout << +result.value() << " | underflow status: false" << std::endl;
    }
    else
    {
        std::cout << "Operation prevented | underflow status: true" << std::endl;
    }
}

void do_overflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Overflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // Testing C++ primitive types
    // signed integers
    test_overflow<char>();
    test_overflow<wchar_t>();
    test_overflow<short int>();
    test_overflow<int>();
    test_overflow<long>();
    test_overflow<long long>();

    // unsigned integers
    test_overflow<unsigned char>();
    test_overflow<unsigned short int>();
    test_overflow<unsigned int>();
    test_overflow<unsigned long>();
    test_overflow<unsigned long long>();

    // real numbers
    test_overflow<float>();
    test_overflow<double>();
    test_overflow<long double>();
}

void do_underflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Underflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // Testing C++ primitive types
    // signed integers
    test_underflow<char>();
    test_underflow<wchar_t>();
    test_underflow<short int>();
    test_underflow<int>();
    test_underflow<long>();
    test_underflow<long long>();

    // unsigned integers
    test_underflow<unsigned char>();
    test_underflow<unsigned short int>();
    test_underflow<unsigned int>();
    test_underflow<unsigned long>();
    test_underflow<unsigned long long>();

    // real numbers
    test_underflow<float>();
    test_underflow<double>();
    test_underflow<long double>();
}

/// <summary>
/// Entry point into the application
/// </summary>
/// <returns>0 when complete</returns>
int main()
{
    //  create a string of "*" to use in the console
    const std::string star_line = std::string(50, '*');

    std::cout << "Starting Numeric Underflow / Overflow Tests!" << std::endl;

    // run the overflow tests
    do_overflow_tests(star_line);

    // run the underflow tests
    do_underflow_tests(star_line);

    std::cout << std::endl << "All Numeric Underflow / Overflow Tests Complete!" << std::endl;

    return 0;
}
