#include "PmergeMe.hpp"

template< typename T >
void printNumbers( T const & sequence )
{
	for (auto const & i : sequence)
	{
		std::cout << i << " ";
	}
}

template< typename T >
void printSequence( T const & sequence, std::string const & label )
{
	std::cout << label << ": ";
	printNumbers(sequence);
	std::cout << std::endl;
}

template< typename T >
std::chrono::duration<double> measureTime( T & sequence, PmergeMe & sorter )
{
	auto start = std::chrono::steady_clock::now();
	sorter.sort(sequence);
	auto end = std::chrono::steady_clock::now();
	
	std::chrono::duration<double> duration = end - start;
	
	return duration;
}

template< typename T >
void printTime( T const & sequence, std::chrono::duration<double> const & duration, std::string const & containerName )
{
	std::cout << std::fixed << std::setprecision(6);
	std::cout << "Time to process a range of " << sequence.size() 
			  << " elements with " << containerName << " : " 
			  << duration.count() << " us" << std::endl;
}

static int parse_argument( char *argument )
{
	int number;
	size_t pos;
	std::string value = argument;

	try
	{
		number = std::stoi(value, &pos);
	}
	catch (std::out_of_range const &)
	{
		throw std::invalid_argument("number too large (integer overflow).");
	}
	catch (std::exception const &)
	{
		throw std::invalid_argument("invalid input format.");
	}

	if (pos != value.length())
	{
		throw std::invalid_argument("contains non-digit characters.");
	}
	if (number < 0)
	{
		throw std::invalid_argument("negative number not allowed.");
	}

	return number;
}

int main( int argc, char **argv )
{
	if (argc < 2)
	{
		std::cerr << "Error: no input provided.\n" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 1;
	}

	std::vector<int>	vector_sequence;
	std::list<int>		list_sequence;

	vector_sequence.reserve(argc - 1);

	try
	{
		for (int i = 1; i < argc; i++)
		{
			int num = parse_argument(argv[i]);
			vector_sequence.push_back(num);
			list_sequence.push_back(num);
		}
	}
	catch (std::exception const & e)
	{
		std::cerr << "Error: " << e.what() << "\n" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 1;
	}

	PmergeMe	pm;

	printSequence(vector_sequence, "Before");

	std::chrono::duration<double> vec_duration = measureTime(vector_sequence, pm);
	std::chrono::duration<double> list_duration = measureTime(list_sequence, pm);

	if (!std::is_sorted(vector_sequence.begin(), vector_sequence.end()))
	{
		std::cerr << "Error: vector sequence is not sorted!" << std::endl;
		return 1;
	}
	if (!std::is_sorted(list_sequence.begin(), list_sequence.end()))
	{
		std::cerr << "Error: list sequence is not sorted!" << std::endl;
		return 1;
	}

	printSequence(vector_sequence, "After ");

	printTime(vector_sequence, vec_duration, "std::vector");
	printTime(list_sequence, list_duration, "std::list  ");

	return 0;
}