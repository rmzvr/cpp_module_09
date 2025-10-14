#include "RPN.hpp"
#include <iostream>
#include <string>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

static int	totalTests = 0;

static void	runTest( std::string const & name, std::string const & input )
{
	totalTests++;
	
	std::cout << "\n" << BLUE << "Test " << totalTests << ": " << name << RESET << "\n";
	std::cout << "Input: " << YELLOW << "\"" << input << "\"" << RESET << "\n";
	std::cout << "Result: ";
	RPN rpn(input);
}

static void	runAllTests()
{
	std::cout << "\n" << YELLOW << "=== Valid Expressions (Baseline) ===" << RESET << "\n";
	runTest("Simple addition", "8 9 +");
	runTest("Simple subtraction", "9 8 -");
	runTest("Simple multiplication", "2 3 *");
	runTest("Simple division", "8 2 /");
	runTest("Complex expression", "8 9 * 9 - 9 - 9 - 4 - 1 +");
	runTest("Multiple operations", "7 7 * 7 -");
	runTest("Chain operations", "1 2 + 3 + 4 +");
	
	std::cout << "\n" << YELLOW << "=== Invalid Format Tests ===" << RESET << "\n";
	runTest("No spaces between tokens", "89+");
	runTest("Missing space before operator", "8 9+");
	runTest("Missing space after operator", "8 +9");
	runTest("Double spaces", "8  9 +");
	runTest("Leading space", " 8 9 +");
	
	std::cout << "\n" << YELLOW << "=== Invalid Token Tests ===" << RESET << "\n";
	runTest("Letter in expression", "8 a +");
	runTest("Special character", "8 9 @");
	runTest("Multiple digits", "10 20 +");
	runTest("Negative number", "-5 3 +");
	runTest("Float number", "8.5 9 +");
	runTest("Invalid operator", "8 9 %");
	
	std::cout << "\n" << YELLOW << "=== Insufficient Operands Tests ===" << RESET << "\n";
	runTest("Only one number", "5");
	runTest("Operator without operands", "+");
	runTest("One number, one operator", "5 +");
	runTest("Too many operators", "5 6 + +");
	runTest("Operator first", "+ 5 6");
	
	std::cout << "\n" << YELLOW << "=== Too Many Operands Tests ===" << RESET << "\n";
	runTest("Two numbers, no operator", "5 6");
	runTest("Three numbers, one operator", "5 6 7 +");
	runTest("Extra operand at end", "5 6 + 7");
	
	std::cout << "\n" << YELLOW << "=== Division Edge Cases ===" << RESET << "\n";
	runTest("Division by zero", "5 0 /");
	runTest("Zero divided by number", "0 5 /");
	
	std::cout << "\n" << YELLOW << "=== Empty/Whitespace Tests ===" << RESET << "\n";
	runTest("Empty string", "");
	runTest("Only spaces", "   ");
	runTest("Only plus operator", "+");
	runTest("Only minus operator", "-");
	runTest("Only multiply operator", "*");
	runTest("Only divide operator", "/");
	
	std::cout << "\n" << YELLOW << "=== Complex Valid Expressions ===" << RESET << "\n";
	runTest("Nested operations", "1 2 + 3 4 + *");
	runTest("All operators", "8 2 / 3 + 2 *");
	runTest("Single digit result", "5 4 -");
	runTest("Zero result", "5 5 -");
	
	std::cout << "\n" << YELLOW << "=== Order of Operations Tests ===" << RESET << "\n";
	runTest("Subtraction order", "9 3 -");
	runTest("Division order", "8 2 /");
	runTest("Mixed operations 1", "2 3 + 4 *");
	runTest("Mixed operations 2", "2 3 4 + *");
}

int main( int argc, char **argv )
{
	if (argc == 2 && std::string(argv[1]) == "--test") {
		runAllTests();
		return 0;
	}
	
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <expression>" << std::endl;
		std::cerr << "       " << argv[0] << " --test (run all tests)" << std::endl;
		return 1;
	}
	
	RPN rpn(argv[1]);
	return 0;
}
