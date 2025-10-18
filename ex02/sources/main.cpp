#include "PmergeMe.hpp"

void	printNumbersVector( std::vector<int> const & numbers )
{
	for (auto &&i : numbers)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

void	printNumbersPairs( std::vector<std::pair<int, int>> const & numbers )
{
	for (auto &&i : numbers)
	{
		std::cout << "[" << i.first << " " << i.second << "] ";
	}
	std::cout << std::endl;
}

/*
	* The Jacobsthal sequence starts as: 0, 1, 1, 3, 5, 11, 21, ...
	* In the Ford–Johnson (merge–insertion) algorithm, the first two numbers (0 and 1)
	* are skipped because:
	*   - J₀ = 0 is not a valid index.
	*   - J₁ = 1 corresponds to b₁, which is already part of the main chain and 
	*     doesn’t need to be inserted again.
	* Therefore, we start generating from J₂ onward (1, 3, 5, 11, ...).
*/
std::vector<int>	get_jacobsthal_sequence( int n , int c )
{
	std::vector<int>	sequence;
	int					prev1 = 1;
	int					prev2 = 0;

	for (int i = 3; i <= n; ++i)
	{
		int	curr = prev1 + 2 * prev2;
		if (curr >= c)
			break;
		prev2 = prev1;
		prev1 = curr;
		sequence.push_back(curr);
	}
	return sequence;
}

std::vector<int>	sortSecondaryChain( std::vector<int> container )
{
	size_t				container_size = container.size();
	std::vector<int>	jacobsthal_sequence = get_jacobsthal_sequence(33, container_size);
	size_t				jacobsthal_sequence_size = jacobsthal_sequence.size();
	std::vector<int>	sorted_secondary_chain;

	for (size_t i = 0; i < jacobsthal_sequence_size; ++i)
	{
		int	range_start_index = jacobsthal_sequence.at(i) - 1;
		int	range_end_index;
		if (i + 1 >= jacobsthal_sequence_size)
		{
			range_end_index = container_size;
		}
		else
		{
			range_end_index = jacobsthal_sequence.at(i + 1) - 1;
		}

		for (int i = range_end_index - 1; i >= range_start_index; --i)
		{
			sorted_secondary_chain.push_back(container.at(i));
		}
	}
	return sorted_secondary_chain;
}

void	binaryInsertionSort( std::vector<int> & container, int n )
{
	int	left = 0;
	int	right = container.size();
	int	pivot = left + (right - left) / 2;
	while (left < right)
	{
		if (container.at(pivot) < n)
			left = pivot + 1;
		else
			right = pivot;
		pivot = left + (right - left) / 2;
	}
	container.insert(container.begin() + pivot, n);
}

std::vector<int>	mergeInsertSort( std::vector<int> const & numbers )
{
	std::vector<std::pair<int, int>>	pairs;
	std::vector<int>					main_chain;
	std::vector<int>					secondary_chain;

	for (std::vector<int>::const_iterator curr_it = numbers.begin(); curr_it < numbers.end(); curr_it += 2)
	{
		std::vector<int>::const_iterator	next_it = curr_it + 1;

		if (next_it == numbers.end())
		{
			break;
		}

		int	smaller = std::min(*curr_it, *next_it);
		int	bigger = std::max(*curr_it, *next_it);

		std::pair<int, int>	pair{smaller, bigger};
		pairs.push_back(pair);
	}

	std::sort(pairs.begin(), pairs.end(), [](std::pair<int, int> const & pair1, std::pair<int, int> const & pair2){ return pair1.second < pair2.second; });
	
	for (std::vector<std::pair<int, int>>::iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		if (it == pairs.begin())
		{
			main_chain.push_back(it->first); //! FIX RECURSION
		}
		
		main_chain.push_back(it->second);
		
		if (it != pairs.begin())
		{
			secondary_chain.push_back(it->first);
		}
	}

	if (numbers.size() % 2 != 0)
	{
		secondary_chain.push_back(numbers.back());
	}

	// std::cout << "iter" << i << "main_chain.size(): " << main_chain.size() << std::endl;
	// if (main_chain.size() > 2)
	// 	main_chain = mergeInsertSort(main_chain, i + 1);

	secondary_chain = sortSecondaryChain(secondary_chain);

	for (auto &i : secondary_chain)
	{
		binaryInsertionSort(main_chain, i);
	}
	// printNumbersVector(main_chain);

	return main_chain;
}

int main( int argc, char **argv )
{
	(void) argc;
	(void) argv;

	std::vector<int>	numbers{ 3, 5 };

	std::vector<int> sorted = mergeInsertSort(numbers);

	std::cout <<std::boolalpha << std::is_sorted(sorted.begin(), sorted.end()) << std::endl;

	return 0;
}
