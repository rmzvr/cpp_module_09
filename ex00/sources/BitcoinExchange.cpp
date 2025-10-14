#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{}

BitcoinExchange::BitcoinExchange( std::string const &filepath )
{
	this->_parseCSVFile();
	try
	{
		this->_processInputFile(filepath);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
	}
}

BitcoinExchange::~BitcoinExchange()
{}

void BitcoinExchange::_parseCSVFile()
{
	std::ifstream	file(this->CSV_FILE_PATH);

	if (file.is_open() == false)
	{
		throw FailedToOpenFileException();
	}

	std::string	line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::string			date;
		std::string			rate_str;
		std::istringstream	line_stream(line);

		if (std::getline(line_stream, date, ',') && std::getline(line_stream, rate_str))
		{
			date = this->_trim(date);
			rate_str = this->_trim(rate_str);

			if (date.empty() || rate_str.empty())
				continue;

			try
			{
				std::size_t pos;
				double rate = std::stod(rate_str, &pos);
				
				if (pos != rate_str.length())
				{
					std::cerr << "Warning: Invalid rate in CSV: " << line << std::endl;
					continue;
				}

				this->_exchangeData[date] = rate;
			}
			catch(const std::exception& e)
			{
				std::cerr << "Warning: Failed to parse CSV line: " << line << std::endl;
			}
		}
	}

	if (this->_exchangeData.empty())
	{
		throw std::runtime_error("CSV file is empty or invalid");
	}
}

void BitcoinExchange::_processInputFile( std::string const &filepath )
{
	std::ifstream	file(filepath);

	if (file.is_open() == false)
	{
		throw FailedToOpenFileException();
	}

	std::string	line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		this->_processInputLine(line);
	}
}

void BitcoinExchange::_processInputLine( std::string const & line )
{
	std::string			date;
	std::string			value_string;
	try
	{
		double				value;
		double				rate;

		if (this->_parseLine(line, date, value_string) == false)
		{
			throw BadInputException(line);
		}

		this->_validateDate(date);

		value = this->_parseNumericValue(value_string);

		std::map<std::string, double>::iterator it = this->_findRecord(date);
		rate = it->second;

		std::cout << date << " => " << value << " = " << (rate * value) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

bool BitcoinExchange::_parseLine( std::string const & line, std::string & date, std::string & value ) const
{
	std::size_t pos = line.find(this->DELIMITER);
	
	if (pos == std::string::npos)
	{
		return false;
	}
	
	date = line.substr(0, pos);
	value = line.substr(pos + this->DELIMITER.length());
	
	date = this->_trim(date);
	value = this->_trim(value);
	
	return !date.empty() && !value.empty();
}

void BitcoinExchange::_validateDate(std::string const &date) const
{
	tm	tm = {};
	std::istringstream	date_in_stream(date);

	date_in_stream >> std::get_time(&tm, "%Y-%m-%d");

	if (date_in_stream.fail() == true)
	{
		throw InvalidDateException(date);
	}

	if (tm.tm_mon < 0 || tm.tm_mon > 11 || 
		tm.tm_mday < 1 || tm.tm_mday > 31)
	{
		throw InvalidDateException(date);
	}

	int year = tm.tm_year + 1900;
	if (year < 2009)
	{
		throw InvalidDateException(date);
	}
}

double BitcoinExchange::_parseNumericValue( std::string const &str ) const
{
	double		result;
	std::size_t	pos;

	result = std::stod(str, &pos);

	if (pos != str.length())
	{
		throw BadInputException(str);
	}

	if (result < 0)
	{
		throw OnlyPositiveNumberException();
	}

	if (result > 1000)
	{
		throw TooLargeNumberException();
	}

	return result;
}

std::map<std::string, double>::iterator BitcoinExchange::_findRecord( std::string const &date )
{
	if (this->_exchangeData.empty())
	{
		throw DateOutOfRangeException();
	}

	std::map<std::string, double>::iterator it = this->_exchangeData.lower_bound(date);

	if (it != this->_exchangeData.end() && it->first == date)
	{
		return it;
	}

	if (it == this->_exchangeData.begin())
	{
		throw DateOutOfRangeException();
	}

	--it;
	return it;
}

std::string BitcoinExchange::_trim( std::string const & str ) const
{
	size_t first = str.find_first_not_of(this->WHITE_SPACES);
	if (first == std::string::npos)
		return "";

	size_t last = str.find_last_not_of(this->WHITE_SPACES);
	return str.substr(first, (last - first + 1));
}


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

BadInputException::BadInputException(std::string const &line)
	: _message("bad input => " + line)
{}

const char * BadInputException::what() const noexcept
{
	return _message.c_str();
}

const char * DateOutOfRangeException::what() const noexcept
{
	return "number out of range.";
}

InvalidDateException::InvalidDateException(std::string const &date)
	: _message("bad input => " + date)
{}

const char * InvalidDateException::what() const noexcept
{
	return _message.c_str();
}
