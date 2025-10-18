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

std::vector<int>	mergeInsertSort( std::vector<int> & numbers, int & comparisons )
{
	std::vector<std::pair<int, int>>	pairs;
	std::vector<int>					main_chain;
	std::vector<int>					secondary_chain;

	//! Base cases
	if (numbers.size() == 0 || numbers.size() == 1)
		return numbers;

	if (numbers.size() == 2)
	{
		if (numbers[0] > numbers[1])
		{
			std::swap(numbers.at(0), numbers.at(1));
			comparisons++;
		}
		return numbers;
	}

	for (std::vector<int>::iterator curr_it = numbers.begin(); curr_it < numbers.end(); curr_it += 2)
	{
		std::vector<int>::iterator	next_it = std::next(curr_it);

		if (next_it == numbers.end())
		{
			break;
		}

		int	smaller = std::min(*curr_it, *next_it);
		comparisons++;
		int	bigger = std::max(*curr_it, *next_it);
		comparisons++;

		main_chain.push_back(bigger); //! bigger

		secondary_chain.push_back(smaller); //! smaller
	}

	if (numbers.size() % 2 != 0)
		secondary_chain.push_back(numbers.back()); //! leftover

	if (secondary_chain.empty() == false)
		main_chain = mergeInsertSort(main_chain, comparisons);

	secondary_chain = sortSecondaryChain(secondary_chain);

	for (auto &i : secondary_chain)
	{
		binaryInsertionSort(main_chain, i);
	}

	return main_chain;
}

int main( int argc, char **argv )
{
	(void) argc;
	(void) argv;

	int					comparisons = 0;

	std::vector<int>	numbers{ 2, 1, 3, 5, 4, 7, 6 };

	std::vector<int>	sorted = mergeInsertSort(numbers, comparisons);

	std::cout << comparisons << std::endl;

	std::cout <<std::boolalpha << std::is_sorted(sorted.begin(), sorted.end()) << std::endl;

	return 0;
}
