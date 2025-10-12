#include "BitcoinExchange.hpp"

void BitcoinExchange::processCSVFile()
{
	std::ifstream	file("data/data.csv");

	if (file.is_open() == false)
	{
		std::cerr << "Failed to open file." << std::endl;
		return ;
	}

	std::string	line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::string			date;
		std::string			exchange_rate;
		std::istringstream	ss(line);
		if (std::getline(ss, date, ',') && std::getline(ss, exchange_rate, ','))
		{
			try
			{
				this->_exchangeRates[date] = std::stof(exchange_rate);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
	}

	tm	tm = {};
	std::istringstream	ss("2009-01-01");
	ss >> std::get_time(&tm, "%Y-%m-%d");

	if (ss.fail())
	{
		throw std::runtime_error("Date parsing failed!");
	}

	std::ostringstream	date_to_find;
	std::map<std::string, float>::iterator it = this->_exchangeRates.end();
	
	std::mktime(&tm);
	date_to_find << std::put_time(&tm, "%Y-%m-%d");
	it = this->_exchangeRates.lower_bound(date_to_find.str());
	if (it->first == date_to_find.str())
	{
		std::cout << date_to_find.str() << std::endl;
	}
	else
	{
		if (this->_exchangeRates.begin()->first > date_to_find.str())
		{
			std::cout << "Out of range!" << std::endl;
		}
		else if (this->_exchangeRates.rbegin()->first < date_to_find.str())
		{
			std::cout << this->_exchangeRates.rbegin()->first << std::endl;
		}
		else
		{
			--it;
			std::cout << it->first << std::endl;
		}
	}
}
