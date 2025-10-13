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

void BitcoinExchange::_validateDate(std::string const &date)
{
	tm	tm = {};
	std::istringstream	date_in_stream(date);

	date_in_stream >> std::get_time(&tm, "%Y-%m-%d");

	if (date_in_stream.fail() == true)
	{
		throw BadInputException();
	}
}

double BitcoinExchange::_parseNumericValue( std::string const &str )
{
	double		result;
	std::size_t	pos;

	try
	{
		result = std::stod(str, &pos);
		if (pos != str.length())
		{
			throw BadInputException(); //! FIX
		}
	}
	catch(...)
	{
		throw BadInputException();
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

	while (std::getline(file, line))
	{
		std::string			date;
		std::string			rate;
		std::istringstream	line_stream(line);

		if (std::getline(line_stream, date, ',') && std::getline(line_stream, rate))
		{
			try
			{
				this->_exchangeData[date] = std::stod(rate);
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}
}

void BitcoinExchange::_parseInputFile( std::string const &filename )
{
	std::ifstream	file(filename);

	if (file.is_open() == false)
	{
		throw FailedToOpenFileException();
	}

	std::string	line;
	std::getline(file, line); //! ADD VALIDATION OF HEADER

	while (std::getline(file, line))
	{
		std::string			date;
		std::string			amount;
		std::istringstream	line_stream(line);

		if (std::getline(line_stream, date, ',') && std::getline(line_stream, amount))
		{
			this->_inputData[date] = amount;
			// std::cerr << "Error: " << e.what() << std::endl; //! HANDLE INVALID LINE READ
		}
	}
}

std::map<std::string, double>::iterator BitcoinExchange::_findRecord( std::string const &date )
{
	std::map<std::string, double>::iterator it = this->_exchangeData.lower_bound(date);

	if (it->first == date)
	{
		return it;
	}

	if (this->_exchangeData.begin()->first > date)
	{
		throw DateOutOfRangeException();
	}

	if (std::prev(this->_exchangeData.end())->first < date)
	{
		return std::prev(this->_exchangeData.end());
	}

	--it;
	return it;
}

void BitcoinExchange::_validateInputData()
{
	
	while (std::getline(file, line))
	{
		std::string			date;
		std::string			amount;
		double				amount_numeric;
		std::istringstream	line_in_stream(line);

		try
		{
			if (!std::getline(line_in_stream, date, '|')) //! ADD VALIDATION FOR " | "
			{
				throw BadInputException();
			}

			this->_validateDate(date);
			std::cout << date << " => ";
		}
		catch(const BadInputException & e)
		{
			std::cerr << "Error: " << e.what() << date << std::endl;
		}

		if (std::getline(line_in_stream, amount))
		{
			try
			{
				amount_numeric = this->_parseNumericValue(amount);
				this->_exchangeData[date] = amount_numeric;
				std::cout << amount_numeric << std::endl;
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}

}
