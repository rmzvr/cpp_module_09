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
	using	map = std::map<std::string, double>;
	private:
		
		map	_exchangeRates;

		// void	_validateNumericValue( std::string str );
		public:
		// BitcoinExchange();
		// ~BitcoinExchange();
		double	_parseNumericValue( std::string const & str );
		void	_parseCSVFile();
		void	_parseInputFile( std::string const & filename );
		map::iterator	_findRecord( std::string const & date_value );

};
