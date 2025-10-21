#pragma once

#include <map>
#include <ctime>
#include <regex>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

class BitcoinExchange
{
	using	ExchangeRateMap = std::multimap<std::string, double>;

	private:
		BitcoinExchange();
		BitcoinExchange( BitcoinExchange const & src ) = delete;
		BitcoinExchange & operator=( BitcoinExchange const & rhs ) = delete;

		ExchangeRateMap	_exchangeData;

		std::string const	DELIMITER = " | ";
		std::string const	WHITE_SPACES = " \t\n\a";
		std::string const	CSV_FILE_PATH = "data/data.csv";

		void					_parseCSVFile();

		void					_processInputFile( std::string const & filepath );
		void					_processInputLine( std::string const & line );

		bool					_parseLine( std::string const & line, std::string & date, std::string & value ) const;

		void					_validateDate( std::string const & date ) const;
		double					_parseNumericValue( std::string const & str ) const;

		ExchangeRateMap::iterator	_findRecord( std::string const & date );

		std::string				_trim( std::string const & str ) const;

	public:
		BitcoinExchange( std::string const & filepath );
		~BitcoinExchange();
};
