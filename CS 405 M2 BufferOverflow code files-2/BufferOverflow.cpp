// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>
#include <string>

int main()
{
  std::cout << "Buffer Overflow Example" << std::endl;

  // The account number remains unchanged and is still declared directly before
  // the variable used for input, as required by the assignment instructions.
  const std::string account_number = "CharlieBrown42";
  std::string user_input;

  // A 20-character char buffer can safely store only 19 user characters plus
  // the required null terminator. This limit preserves the original input size
  // rule while using std::string to prevent writing past a fixed-size buffer.
  const std::size_t MAX_ALLOWED_INPUT = 19;

  std::cout << "Enter a value: ";
  std::getline(std::cin, user_input);

  // Detect an attempted overflow before using the data. If the input is too
  // long, notify the user and reject the value instead of truncating silently.
  if (user_input.length() > MAX_ALLOWED_INPUT)
  {
    std::cout << "Input rejected: value is too long. Please enter "
              << MAX_ALLOWED_INPUT << " characters or fewer." << std::endl;
    user_input = "";
  }

  std::cout << "You entered: " << user_input << std::endl;
  std::cout << "Account Number = " << account_number << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
