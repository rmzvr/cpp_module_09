#include "PmergeMe.hpp"

void	printNumbersVector( std::vector<int> const & numbers )
{
	for (auto &&i : numbers)
	{
		std::cout << i << " ";
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

void	binaryInsertion( std::vector<int> & container, int n, int & comparisons )
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
		comparisons++;
		pivot = left + (right - left) / 2;
	}

	container.insert(container.begin() + pivot, n);
}

void	swapTwoElementsContainer( std::vector<int> & numbers, int & comparisons )
{
	if (numbers[0] > numbers[1])
	{
		std::swap(numbers[0], numbers[1]);
		comparisons++;
	}
}

void	splitNumbersToMainAndSecondaryChains( std::vector<int> const & numbers, std::vector<int> & main_chain, std::vector<int> & secondary_chain, int & comparisons )
{
	for (std::vector<int>::const_iterator curr_it = numbers.begin(); curr_it < numbers.end(); curr_it += 2)
	{
		std::vector<int>::const_iterator	next_it = std::next(curr_it);

		if (next_it == numbers.end())
		{
			break;
		}

		int	a = *curr_it;
		int	b = *next_it;

		int	smaller;
		int	bigger;

		if (a > b)
		{
			bigger = a;
			smaller = b;
		}
		else
		{
			bigger = b;
			smaller = a;
		}
		comparisons++;

		main_chain.push_back(bigger);
		secondary_chain.push_back(smaller);
	}

	if (numbers.size() % 2 != 0)
		secondary_chain.push_back(numbers.back());
}

void	performBinaryInsertionWithJacobsthalSequence( std::vector<int> & main_chain, std::vector<int> & secondary_chain, int & comparisons )
{
	size_t				secondary_chain_size = secondary_chain.size();
	std::vector<int>	jacobsthal_sequence = get_jacobsthal_sequence(33, secondary_chain_size);
	size_t				jacobsthal_sequence_size = jacobsthal_sequence.size();

	for (size_t i = 0; i < jacobsthal_sequence_size; ++i)
	{
		int	range_start_index = jacobsthal_sequence[i] - 1;
		int	range_end_index;
		if (i + 1 >= jacobsthal_sequence_size)
		{
			range_end_index = secondary_chain_size;
		}
		else
		{
			range_end_index = jacobsthal_sequence[i + 1] - 1;
		}

		for (int j = range_end_index - 1; j >= range_start_index; --j)
		{
			binaryInsertion(main_chain, secondary_chain[j], comparisons);
		}
	}
}

void	mergeInsertSort( std::vector<int> & numbers, int & comparisons )
{
	size_t	number_size = numbers.size();

	if (number_size == 0 || number_size == 1)
		return ;

	if (number_size == 2)
	{
		swapTwoElementsContainer(numbers, comparisons);
		return ;
	}

	std::vector<int>	main_chain;
	std::vector<int>	secondary_chain;

	splitNumbersToMainAndSecondaryChains(numbers, main_chain, secondary_chain, comparisons);

	mergeInsertSort(main_chain, comparisons);

	performBinaryInsertionWithJacobsthalSequence(main_chain, secondary_chain, comparisons);

	numbers = main_chain;
}

int main( int argc, char **argv )
{
	(void) argc;
	(void) argv;

	std::vector<int>	numbers{ 15, 12, 7, 9, 10, 3, 1, 14, 8, 5, 11, 6, 2, 4, 13 };

	int	comparisons = 0;

	mergeInsertSort(numbers, comparisons);

	std::cout << "comparisons: " << comparisons << std::endl;
	std::cout <<std::boolalpha << std::is_sorted(numbers.begin(), numbers.end()) << std::endl;

	return 0;
}
