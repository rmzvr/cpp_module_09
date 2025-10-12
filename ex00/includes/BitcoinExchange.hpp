#pragma once

#include <map>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

class BitcoinExchange
{
	private:
		std::map<std::string, float>	_exchangeRates;
	public:
		// BitcoinExchange();
		// ~BitcoinExchange();

		void	processCSVFile();
};
