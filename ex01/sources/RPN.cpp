#include "RPN.hpp"

RPN::RPN( std::string const &expression )
{
	if (!this->_validateExpression(expression))
	{
		std::cerr << "Error" << std::endl;
		return ;
	}

	if (!this->_calculateExpression(expression))
	{
		std::cerr << "Error" << std::endl;
		return ;
	}

	if (this->_stack.size() != 1)
	{
		std::cerr << "Error" << std::endl;
		return ;
	}

	std::cout << this->_popFromStack() << std::endl;
}

RPN::~RPN()
{}

bool RPN::_validateExpression( std::string const & expression )
{
	if (expression.length() == 1 && !std::isdigit(expression.at(0)))
		return false;

	for (size_t i = 1; i < expression.length(); i += 2)
	{
		if (!std::isspace(expression.at(i)))
			return false;

		char const	prevChar = expression.at(i - 1);
		if (!std::isdigit(prevChar) && !this->_isOperator(prevChar))
			return false;
	}
	return true;
}

void RPN::_pushIntToStack( char c )
{
	this->_stack.push(c - '0');
}

int RPN::_popFromStack()
{
	int	a = this->_stack.top();
	this->_stack.pop();
	return a;
}

bool RPN::_isOperator( char c )
{
	return std::string(this->_OPERATORS).find(c) != std::string::npos;
}

int RPN::_performOperation( char op, int b, int a )
{
	if (op == '+')
		return b + a;
	else if (op == '-')
		return b - a;
	else if (op == '*')
		return b * a;
	else // op == '/'
		return b / a;
}

bool	RPN::_calculateExpression(std::string const &expression)
{
	char	c;
	std::istringstream	expression_stream(expression);

	while (expression_stream >> c)
	{
		if (isdigit(c))
		{
			this->_pushIntToStack(c);
		}
		else if (this->_isOperator(c))
		{
			if (this->_stack.size() < 2)
				return false;

			int const	a = this->_popFromStack();
			int const	b = this->_popFromStack();

			if (c == '/' && a == 0)
				return false;

			int const	result = this->_performOperation(c, b, a);
			this->_stack.push(result);
		}
		else
		{
			return false;
		}
	}
	return true;
}
