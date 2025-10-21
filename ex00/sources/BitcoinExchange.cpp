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
		throw std::runtime_error("could not open file.");
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
			try
			{
				std::size_t pos;
				double rate = std::stod(rate_str, &pos);

				this->_exchangeData.emplace(date, rate);
			}
			catch(std::exception const & e)
			{
				std::cerr << "Failed to parse CSV line: " << line << std::endl;
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
		throw std::runtime_error("could not open file.");
	}

	std::string	line;
	std::getline(file, line);

	if (line != "date | value")
	{
		throw std::runtime_error("invalid header => \"" + line + "\"");
	}

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
		double	value;
		double	rate;

		if (line.empty() || this->_parseLine(line, date, value_string) == false)
		{
			throw std::invalid_argument("bad input => \"" + line + "\"");
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
	
	std::string	trimmed_value = this->_trim(value);
	if (trimmed_value.length() != value.length())
	{
		throw std::invalid_argument("bad input => \"" + value + "\"");
	}

	return !date.empty() && !value.empty();
}

void BitcoinExchange::_validateDate(std::string const &date) const
{
	std::regex date_pattern("^\\d{4}-\\d{2}-\\d{2}$");

	if (!std::regex_match(date, date_pattern))
	{
		throw std::invalid_argument("invalid date => \"" + date + "\"");
	}

	std::istringstream	date_in_stream(date);

	tm	tm = {};
	date_in_stream >> std::get_time(&tm, "%Y-%m-%d");

	if (date_in_stream.fail() || !date_in_stream.eof())
	{
		throw std::invalid_argument("invalid date => \"" + date + "\"");
	}

	int original_year = tm.tm_year;
	int original_mon = tm.tm_mon;
	int original_mday = tm.tm_mday;

	std::mktime(&tm);

	if (tm.tm_year != original_year || 
		tm.tm_mon != original_mon || 
		tm.tm_mday != original_mday)
	{
		throw std::invalid_argument("invalid date => \"" + date + "\"");
	}
}

double BitcoinExchange::_parseNumericValue( std::string const &str ) const
{
	double		result;
	std::size_t	pos;

	try
	{
		result = std::stod(str, &pos);
	}
	catch(const std::exception& e)
	{
		throw std::invalid_argument("bad input => \"" + str + "\"");
	}
	

	if (pos != str.length())
	{
		throw std::invalid_argument("bad input => \"" + str + "\"");
	}

	if (result < 0)
	{
		throw std::invalid_argument("not a positive number.");
	}

	if (result > 1000)
	{
		throw std::out_of_range("too large a number.");
	}

	return result;
}

std::map<std::string, double>::iterator BitcoinExchange::_findRecord( std::string const &date )
{
	if (this->_exchangeData.empty())
	{
		throw std::out_of_range("date out of range.");
	}

	std::map<std::string, double>::iterator it = this->_exchangeData.lower_bound(date);

	if (it != this->_exchangeData.end() && it->first == date)
	{
		return it;
	}

	if (it == this->_exchangeData.begin())
	{
		throw std::out_of_range("date out of range.");
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
