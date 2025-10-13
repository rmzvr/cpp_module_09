#include "BitcoinExchange.hpp"
// #include "ScalarConverter.hpp"

const char * FailedToOpenFileException::what() const noexcept
{
	return "could not open file.";
}

const char * OnlyPositiveNumberException::what() const noexcept
{
	return "not a positive number.";
}

const char * TooLargeNumberException::what() const noexcept
{
	return "too large a number.";
}

const char * BadInputException::what() const noexcept
{
	return "bad input => ";
}

const char * DateOutOfRangeException::what() const noexcept
{
	return "number out of range.";
}

double	BitcoinExchange::_parseNumericValue( std::string const & str )
{
	double		result;
	std::size_t	pos;

	try
	{
		result = std::stod(str, &pos);
		if (pos != str.length())
		{
			throw BadInputException();
		}
	}
	catch(...)
	{
		throw BadInputException(); //! ADD DETAILS
	}
	if (result < 0)
	{
		throw OnlyPositiveNumberException();
	}
	else if (result > 1000)
	{
		throw TooLargeNumberException();
	}

	return result;
}

void BitcoinExchange::_parseCSVFile()
{
	std::ifstream	file("data/data.csv");

	if (file.is_open() == false)
	{
		throw FailedToOpenFileException();
	}

	std::string	line;
	std::getline(file, line);
	// double		coverted_exchange_rate;
	
	while (std::getline(file, line))
	{
		std::string			date_value;
		std::string			exchange_rate_value;
		std::istringstream	ss(line);
		if (std::getline(ss, date_value, ',') && std::getline(ss, exchange_rate_value))
		{
			try
			{
				// coverted_exchange_rate = this->_parseNumericValue(exchange_rate_value);
				this->_exchangeRates[date_value] = std::stod(exchange_rate_value);
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << '\n';
			}
		}
	}
}

void BitcoinExchange::_parseInputFile(std::string const &filename)
{
	std::ifstream	file(filename);

	if (file.is_open() == false)
	{
		throw FailedToOpenFileException();
	}
	std::cout << "File opened!" << std::endl;
}

std::map<std::string, double>::iterator BitcoinExchange::_findRecord( std::string const &date_value )
{
	tm	tm = {};
	std::istringstream	ss(date_value);
	ss >> std::get_time(&tm, "%Y-%m-%d");

	if (ss.fail())
	{
		throw BadInputException();
	}

	std::ostringstream	date_to_find;
	std::map<std::string, double>::iterator it = this->_exchangeRates.end();
	
	std::mktime(&tm);
	date_to_find << std::put_time(&tm, "%Y-%m-%d");
	it = this->_exchangeRates.lower_bound(date_to_find.str());

	if (it->first == date_to_find.str())
	{
		// std::cout << date_to_find.str() << std::endl;
		return it;
	}

	if (this->_exchangeRates.begin()->first > date_to_find.str())
	{
		throw DateOutOfRangeException();
	}

	if (std::prev(this->_exchangeRates.end())->first < date_to_find.str())
	{
		// std::cout << std::prev(this->_exchangeRates.end())->first << std::endl;
		return std::prev(this->_exchangeRates.end());
	}

	--it;
	// std::cout << it->first << std::endl;
	return it;
}
