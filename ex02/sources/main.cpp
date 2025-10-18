#include "PmergeMe.hpp"

int main( int argc, char **argv )
{
	(void) argc;
	(void) argv;

	std::vector<int>	vector_sequence{ 15, 12, 7, 9, 10, 3, 1, 14, 8, 5, 11, 6, 2, 4, 13 };
	// std::list<int>		list_sequence{ 15, 12, 7, 9, 10, 3, 1, 14, 8, 5, 11, 6, 2, 4, 13 };

	PmergeMe	pm;

	pm.sort(vector_sequence);
	// pm.sort(list_sequence);

	std::cout << "vector is sorted: " << std::boolalpha << std::is_sorted(vector_sequence.begin(), vector_sequence.end()) << std::endl;
	// std::cout << "list is sorted: " << std::boolalpha << std::is_sorted(list_sequence.begin(), list_sequence.end()) << std::endl;

	return 0;
}
