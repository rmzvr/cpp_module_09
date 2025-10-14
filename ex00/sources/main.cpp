#include "BitcoinExchange.hpp"

#define COUNT_OF_ARGUMENTS 2

int main(int argc, char **argv)
{
	if (argc < COUNT_OF_ARGUMENTS)
	{
		std::cerr << "Error: \n\n";
		std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
		return 1;
	}
	BitcoinExchange	be(argv[1]);
	return 0;
}