#pragma once

#include <map>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

class FailedToOpenFileException : public std::exception
{
	public:
		const char * what() const noexcept override;
};

class OnlyPositiveNumberException : public std::exception
{
	public:
		const char * what() const noexcept override;
};

class TooLargeNumberException : public std::exception
{
	public:
		const char * what() const noexcept override;
};

class BadInputException : public std::exception
{
	public:
		const char * what() const noexcept override;
};

class DateOutOfRangeException : public std::exception
{
	public:
		const char * what() const noexcept override;
};

class BitcoinExchange
{
	using	exchangeMap = std::map<std::string, double>;
	using	inputMap = std::map<std::string, std::string>;

	private:
		exchangeMap	_exchangeData;
		inputMap	_inputData;

	public:
		void			_parseCSVFile();
		void			_parseInputFile( std::string const & filename );

		void			_validateInputData();
		void			_validateDate( std::string const & date );
		double			_parseNumericValue( std::string const & str );

		exchangeMap::iterator	_findRecord( std::string const & date );

};
