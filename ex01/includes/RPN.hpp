#pragma once

#include <iostream>
#include <stack>
#include <sstream>

class RPN
{
	private:
		RPN() = delete;

		std::stack<int>		_stack;
		std::string const	_OPERATORS = "+-*/";

		bool		_validateExpression( std::string const & expression );
		bool		_calculateExpression( std::string const & expression );
		int			_performOperation( char op, int b, int a );

		int			_popFromStack();
		void		_pushIntToStack( char c );

		bool		_isOperator( char c );
	public:
		RPN( std::string const & expression );
		~RPN();
};
