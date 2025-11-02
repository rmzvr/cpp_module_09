#include "PmergeMe.hpp"

static void parseArguments
(
	int argc,
	char **argv,
	std::vector<int> & vector_sequence,
	std::list<int> & list_sequence
)
{
	vector_sequence.reserve(argc - 1);

	for (int i = 1; i < argc; ++i)
	{
		int	num = PmergeMe::parseArgument(argv[i]);
		vector_sequence.push_back(num);
		list_sequence.push_back(num);
	}
}

static void sortAndTrackComparisons
(
	PmergeMe & pm,
	std::vector<int> & vector_sequence,
	std::list<int> & list_sequence,
	std::chrono::duration<double> & vec_duration,
	std::chrono::duration<double> & list_duration,
	int & comparisons
)
{
	PmergeMe::resetComparisons();
	vec_duration = pm.sortAndMeasureTime(vector_sequence);
	comparisons = PmergeMe::getComparisons();
	
	list_duration = pm.sortAndMeasureTime(list_sequence);
}

static void verifySorting
(
	PmergeMe const & pm,
	std::vector<int> const & vector_sequence,
	std::list<int> const & list_sequence
)
{
	if (!pm.verifySorted(vector_sequence))
	{
		throw std::runtime_error("vector sequence is not sorted!");
	}
	if (!pm.verifySorted(list_sequence))
	{
		throw std::runtime_error("list sequence is not sorted!");
	}
}

static void printResults
(
	PmergeMe const & pm,
	std::vector<int> const & vector_sequence,
	std::list<int> const & list_sequence,
	std::chrono::duration<double> const & vec_duration,
	std::chrono::duration<double> const & list_duration
)
{
	pm.printSequence(vector_sequence, "After ");
	pm.printTime(vector_sequence, vec_duration, "std::vector");
	pm.printTime(list_sequence, list_duration, "std::list  ");
}

#ifdef DEBUG
	static void printComparisons
	(
		std::vector<int> const & vector_sequence,
		int comparisons
	)
	{
		int	n = vector_sequence.size();
		int	expectedComparisons = PmergeMe::getExpectedComparisons(n);
		
		std::cout << "\n=== Comparison Analysis for n=" << n << " elements ===" << std::endl;
		std::cout << "Actual comparisons: " << comparisons << std::endl;
		
		if (expectedComparisons != -1)
		{
			std::cout << "Expected max comparisons: " << expectedComparisons << std::endl;
		}
		else
		{
			std::cout << "Expected max comparisons: Unknown (n > 33)" << std::endl;
		}
	}
#endif

int main
(
	int argc,
	char **argv
)
{
	if (argc < 2)
	{
		std::cerr << "Error: no input provided.\n" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 1;
	}

	std::vector<int>	vector_sequence;
	std::list<int>		list_sequence;

	try
	{
		parseArguments(argc, argv, vector_sequence, list_sequence);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Error: " << e.what() << "\n" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 1;
	}

	PmergeMe	pm;

	pm.printSequence(vector_sequence, "Before");

	std::chrono::duration<double>	vec_duration;
	std::chrono::duration<double>	list_duration;
	int								comparisons;

	sortAndTrackComparisons(pm, vector_sequence, list_sequence, vec_duration, list_duration, comparisons);

	try
	{
		verifySorting(pm, vector_sequence, list_sequence);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	printResults(pm, vector_sequence, list_sequence, vec_duration, list_duration);

	#ifdef DEBUG
		printComparisons(vector_sequence, comparisons);
	#endif

	return 0;
}
