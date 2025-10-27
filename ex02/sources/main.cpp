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
	// std::list<int>		list_sequence;

	vector_sequence.reserve(argc - 1);

	try
	{
		for (int i = 1; i < argc; i++)
		{
			int num = PmergeMe::parseArgument(argv[i]);
			vector_sequence.push_back(num);
			// list_sequence.push_back(num);
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

	std::chrono::duration<double> vec_duration = pm.sortAndMeasureTime(vector_sequence);
	// std::chrono::duration<double> list_duration = pm.sortAndMeasureTime(list_sequence);

	if (!pm.verifySorted(vector_sequence))
	{
		pm.printSequence(vector_sequence, "After ");

		std::cerr << "Error: vector sequence is not sorted!" << std::endl;
		return 1;
	}
	// if (!pm.verifySorted(list_sequence))
	// {
	// 	std::cerr << "Error: list sequence is not sorted!" << std::endl;
	// 	return 1;
	// }

	pm.printSequence(vector_sequence, "After ");

	pm.printTime(vector_sequence, vec_duration, "std::vector");
	// pm.printTime(list_sequence, list_duration, "std::list  ");

	return 0;
}
