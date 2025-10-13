#include "BitcoinExchange.hpp"

#define COUNT_OF_ARGUMENTS 2

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: bad arguments\n\n";
		std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
	}
	(void) argc;
	(void) argv;
	BitcoinExchange	be;
	be._parseCSVFile();
	be._parseInputFile(argv[1]);
	std::map<std::string, double>::iterator it = be._findRecord("2012-08-01");
	std::cout << it->first << " => " << it->second << std::endl;
	return 0;
}