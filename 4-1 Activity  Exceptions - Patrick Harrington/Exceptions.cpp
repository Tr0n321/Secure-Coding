// Exceptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

// Custom exception type for application-specific failures. Deriving from
// std::exception keeps the exception compatible with standard C++ handlers.
class CustomApplicationException : public std::exception
{
private:
  std::string message;

public:
  explicit CustomApplicationException(const std::string& errorMessage)
    : message(errorMessage)
  {
  }

  // Override what() so the caller can display a meaningful user-facing message.
  const char* what() const noexcept override
  {
    return message.c_str();
  }
};

bool do_even_more_custom_application_logic()
{
  std::cout << "Running Even More Custom Application Logic." << std::endl;

  // Throw a standard C++ exception to simulate a recoverable lower-level error.
  throw std::runtime_error("Standard exception thrown from even more custom application logic.");

  return true;
}

void do_custom_application_logic()
{
  std::cout << "Running Custom Application Logic." << std::endl;

  // Catch standard exceptions from the nested application logic so processing
  // can continue instead of allowing the application to crash abruptly.
  try
  {
    if (do_even_more_custom_application_logic())
    {
      std::cout << "Even More Custom Application Logic Succeeded." << std::endl;
    }
  }
  catch (const std::exception& exception)
  {
    std::cout << "Standard exception caught in custom application logic: "
      << exception.what() << std::endl;
  }

  // Throw a custom application exception that main() will catch explicitly.
  throw CustomApplicationException("Custom application logic encountered a business-rule failure.");

  std::cout << "Leaving Custom Application Logic." << std::endl;
}

float divide(float numerator, float denominator)
{
  // Use a standard C++ exception to prevent divide-by-zero behavior.
  if (denominator == 0.0f)
  {
    throw std::invalid_argument("Division by zero is not allowed.");
  }

  return (numerator / denominator);
}

void do_division() noexcept
{
  float numerator = 10.0f;
  float denominator = 0.0f;

  // This handler intentionally catches only the exception type thrown by divide().
  // Because this function is noexcept, the exception must be handled here.
  try
  {
    auto result = divide(numerator, denominator);
    std::cout << "divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
  }
  catch (const std::invalid_argument& exception)
  {
    std::cout << "Division error caught: " << exception.what() << std::endl;
  }
}

int main()
{
  std::cout << "Exceptions Tests!" << std::endl;

  // Wrap the main workflow so all expected and unexpected errors display a
  // user-facing message instead of terminating without explanation.
  try
  {
    do_division();
    do_custom_application_logic();
  }
  catch (const CustomApplicationException& exception)
  {
    std::cout << "Custom exception caught in main: " << exception.what() << std::endl;
  }
  catch (const std::exception& exception)
  {
    std::cout << "Standard exception caught in main: " << exception.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "Unknown exception caught in main." << std::endl;
  }

  std::cout << "Exception handling tests completed." << std::endl;

  return 0;
}
