#include "PmergeMe.hpp"

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
			int num = PmergeMe::parseArgument(argv[i]);
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

	pm.printSequence(vector_sequence, "Before");

	// Reset comparison counter before sorting
	PmergeMe::resetComparisons();
	
	std::chrono::duration<double> vec_duration = pm.sortAndMeasureTime(vector_sequence);
	std::chrono::duration<double> list_duration = pm.sortAndMeasureTime(list_sequence);

	// Get the number of comparisons made
	// int actualComparisons = PmergeMe::getComparisons();
	// int n = vector_sequence.size();
	// int expectedComparisons = PmergeMe::getExpectedComparisons(n);
	// bool testPassed = PmergeMe::validateComparisons(n, actualComparisons);

	if (!pm.verifySorted(vector_sequence))
	{
		std::cerr << "Error: vector sequence is not sorted!" << std::endl;
		return 1;
	}
	if (!pm.verifySorted(list_sequence))
	{
		std::cerr << "Error: list sequence is not sorted!" << std::endl;
		return 1;
	}

	pm.printSequence(vector_sequence, "After ");

	pm.printTime(vector_sequence, vec_duration, "std::vector");
	pm.printTime(list_sequence, list_duration, "std::list  ");

	// Print comparison information
	// std::cout << "\nComparison Analysis for n=" << n << " elements:" << std::endl;
	// std::cout << "Actual comparisons: " << actualComparisons << std::endl;
	// if (expectedComparisons != -1)
	// {
	// 	std::cout << "Expected max comparisons: " << expectedComparisons << std::endl;
	// 	std::cout << "Test result: " << (testPassed ? "PASSED" : "FAILED") << std::endl;
	// 	if (!testPassed)
	// 	{
	// 		std::cerr << "Error: Too many comparisons! Expected <= " << expectedComparisons << " but got " << actualComparisons << std::endl;
	// 		return 1;
	// 	}
	// }
	// else
	// {
	// 	std::cout << "Expected max comparisons: Unknown (n > 33)" << std::endl;
	// 	std::cout << "Test result: PASSED (no validation data available)" << std::endl;
	// }

	return 0;
}
