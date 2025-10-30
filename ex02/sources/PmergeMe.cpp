#include "PmergeMe.hpp"

int comparisons = 0; //! Remove

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe( PmergeMe const & other )
{
	(void)other;
}

PmergeMe &	PmergeMe::operator=( PmergeMe const & other )
{
	(void)other;
	return *this;
}

PmergeMe::~PmergeMe()
{}

void PmergeMe::sort( VectorSequence & sequence )
{
	size_t	sequence_size = sequence.size();
	size_t	recursion_depth = 1;
	this->_mergeInsertionSort(sequence, sequence_size, recursion_depth);
}

void PmergeMe::sort( ListSequence & sequence )
{
	this->_mergeInsertionSort(sequence);
}

// Utility functions - Vector
void PmergeMe::printSequence( VectorSequence const & sequence, std::string const & label ) const
{
	std::cout << label << ": ";
	this->_printNumbers(sequence);
}

void PmergeMe::printTime( VectorSequence const & sequence, std::chrono::duration<double> const & duration, std::string const & containerName ) const
{
	std::cout << std::fixed << std::setprecision(6);
	std::cout << "Time to process a range of " << sequence.size() 
				<< " elements with " << containerName << " : " 
				<< duration.count() << " us" << std::endl;
}

bool PmergeMe::verifySorted( VectorSequence const & sequence ) const
{
	return std::is_sorted(sequence.begin(), sequence.end());
}

std::chrono::duration<double> PmergeMe::sortAndMeasureTime( VectorSequence & sequence )
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	this->sort(sequence);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	return end - start;
}

// Utility functions - List
void PmergeMe::printSequence( ListSequence const & sequence, std::string const & label ) const
{
	std::cout << label << ": ";
	this->_printNumbers(sequence);
	std::cout << std::endl;
}

void PmergeMe::printTime( ListSequence const & sequence, std::chrono::duration<double> const & duration, std::string const & containerName ) const
{
	std::cout << std::fixed << std::setprecision(6);
	std::cout << "Time to process a range of " << sequence.size() 
				<< " elements with " << containerName << " : " 
				<< duration.count() << " us" << std::endl;
}

bool PmergeMe::verifySorted( ListSequence const & sequence ) const
{
	return std::is_sorted(sequence.begin(), sequence.end());
}

std::chrono::duration<double> PmergeMe::sortAndMeasureTime( ListSequence & sequence )
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	this->sort(sequence);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	return end - start;
}

// Static utility function

int PmergeMe::parseArgument( char *argument )
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

// Sorting - Vector

size_t	PmergeMe::_findGroupInsertionIndex( VectorSequence const & chain, size_t max_search_range, int target_value, size_t elements_per_group )
{
	int	left = 0;
	int	right = max_search_range / elements_per_group;

	while (left < right)
	{
		int	middle = left + (right - left) / 2;
		int	middle_value = chain[middle * elements_per_group + elements_per_group - 1];

		if (middle_value < target_value)
			left = middle + 1;
		else
			right = middle;

		comparisons++;
	}

	return left * elements_per_group;
}

void	PmergeMe::_insertPendingGroup( VectorSequence & main_chain, VectorSequence const & pending_chain, int i, size_t amount_of_elements_in_member, size_t group_index )
{
		size_t const	group_start = i * amount_of_elements_in_member;
		size_t const	group_end = group_start + amount_of_elements_in_member;

		size_t const	search_limit = (std::pow(2, group_index) - 1) * amount_of_elements_in_member;
		size_t const	max_search_range = std::min(main_chain.size(), search_limit);

		size_t const	largest_element_in_group = pending_chain[group_end - 1];
		size_t const	idx_to_insert = this->_findGroupInsertionIndex(main_chain, max_search_range, largest_element_in_group, amount_of_elements_in_member);

		main_chain.insert(main_chain.begin() + idx_to_insert, pending_chain.begin() + group_start, pending_chain.begin() + group_end);
}

void	PmergeMe::_insertWithJacobsthalSequence1( VectorSequence & sequence, VectorSequence & main_chain, VectorSequence & pending_chain, size_t amount_of_elements_in_member )
{
	VectorSequence	jacobsthal_sequence;
	size_t const	pending_chain_members_amount = pending_chain.size() / amount_of_elements_in_member;

	//! Ford-Johnson algorithm: first insertion searches 2^k-1 = 3 elements
	size_t	group_index = 2;

	this->_generateJacobsthalSequence(jacobsthal_sequence, PmergeMe::MAX_JACOBSTHAL_INDEX, pending_chain_members_amount);

	for (auto it = jacobsthal_sequence.begin(); it != jacobsthal_sequence.end(); ++it)
	{
		int const	range_start = *it - 1;
		int			range_end = pending_chain_members_amount;

		if (std::next(it) != jacobsthal_sequence.end())
			range_end = *std::next(it) - 1;

		for (int i = range_end - 1; i >= range_start; --i)
		{
			this->_insertPendingGroup(main_chain, pending_chain, i, amount_of_elements_in_member, group_index);
		}
		group_index++;
	}

	this->printSequence(main_chain, "\nMain chain after insertion");
	this->printSequence(pending_chain, "Pending chain after insertion");

	this->printSequence(sequence, "\nSequence before main chain assign");

	this->printSequence(main_chain, "Main chain before assign to sequence");
	sequence = main_chain;
	this->printSequence(sequence, "\nSequence after main chain assign");
}

void	PmergeMe::_sortPairs
(
	VectorSequence & sequence,
	size_t amount_of_members_in_pair,
	size_t amount_of_elements_in_member,
	size_t amount_of_elements_to_pair
)
{
	for (
		size_t	i = 0;
		i < amount_of_elements_to_pair;
		i += amount_of_members_in_pair
	)
	{
		size_t const	left_member_start = i;
		size_t const	left_member_end = left_member_start + amount_of_elements_in_member; //! last element excluded == vector.end()
		size_t const	right_member_end = left_member_end + amount_of_elements_in_member; //! last element excluded == vector.end()

		if (sequence[left_member_end - 1] > sequence[right_member_end - 1])
		{
			std::swap_ranges(
				sequence.begin() + left_member_start,
				sequence.begin() + left_member_end,
				sequence.begin() + left_member_end
			);
		}
		comparisons++;
	}
}

void PmergeMe::_mergeInsertionSort( VectorSequence & sequence, size_t sequence_size, size_t recursion_depth )
{
	size_t	amount_of_members_in_pair = 1 << recursion_depth;
	size_t	amount_of_elements_in_member = amount_of_members_in_pair / 2;
	size_t	amount_of_pairs = sequence_size / amount_of_members_in_pair;
	size_t	amount_of_elements_to_pair = amount_of_pairs * amount_of_members_in_pair;

	if (amount_of_pairs < 1)
	{
		return;
	}

	std::cout << "\nrecursion_depth - " << recursion_depth;
	std::cout << "\namount_of_members_in_pair - " << amount_of_members_in_pair;
	std::cout << "\namount_elements_in_member - " << amount_of_elements_in_member;
	std::cout << "\namount_of_pairs - " << amount_of_pairs;
	std::cout << "\namount_of_elements_to_pair - " << amount_of_elements_to_pair;
	std::cout << "\nsequence_size - " << sequence_size;
	std::cout << "\ncomparisons: " << comparisons << std::endl;
	std::cout << "\n" << std::endl;

	this->printSequence(sequence, "Sequence before sorting pairs");

	std::cout << "Sorting pairs..." << std::endl;
	this->_sortPairs(sequence, amount_of_members_in_pair, amount_of_elements_in_member, amount_of_elements_to_pair);

	this->printSequence(sequence, "Sequence after sorting pairs");

	if (amount_of_pairs > 1)
	{
		std::cout << "\nEnter recursion [" << recursion_depth + 1 << "]..." << std::endl;
		this->_mergeInsertionSort(sequence, amount_of_elements_to_pair, recursion_depth + 1);
		std::cout << "\nExit recursion [" << recursion_depth + 1 << "]...\n" << std::endl;
	}

	if (amount_of_pairs < 2 && amount_of_elements_to_pair == sequence_size)
	{
		return;
	}

	VectorSequence	main_chain;
	VectorSequence	pending_chain;

	this->printSequence(sequence, "Sequence before splitting to chains");
	std::cout << "sequence_size - " << sequence_size << std::endl;

	std::cout << "\nSplit sequence to chains..." << std::endl;
	this->_splitToChains(sequence, main_chain, pending_chain, amount_of_members_in_pair, amount_of_elements_in_member, amount_of_elements_to_pair, sequence_size);

	this->printSequence(main_chain, "\nMain chain after splitting");
	this->printSequence(pending_chain, "Pending chain after splitting");

	std::cout << "\nInserting pending chain to main chain..." << std::endl;
	this->_insertWithJacobsthalSequence1(sequence, main_chain, pending_chain, amount_of_elements_in_member);

	std::cout << "\ncomparisons: " << comparisons << std::endl;
}

void PmergeMe::_splitToChains
(
	VectorSequence const & sequence,
	VectorSequence & main_chain,
	VectorSequence & pending_chain,
	size_t amount_of_members_in_pair,
	size_t amount_of_elements_in_member,
	size_t amount_of_elements_to_pair,
	size_t sequence_size
)
{
	for (size_t	i = 0; i < amount_of_elements_to_pair; i += amount_of_members_in_pair)
	{
		size_t const	left_member_start = i;
		size_t const	left_member_end = left_member_start + amount_of_elements_in_member; //! last element excluded == vector.end()
		size_t const	right_member_start = left_member_end;
		size_t const	right_member_end = right_member_start + amount_of_elements_in_member; //! last element excluded == vector.end()

		if (i != 0)
		{
			//! Push rest smaller elements to pending chain
			pending_chain.insert(pending_chain.end(), sequence.begin() + left_member_start, sequence.begin() + left_member_end);
		}
		else
		{
			//! Push first smaller elements to main chain
			main_chain.insert(main_chain.end(), sequence.begin() + left_member_start, sequence.begin() + left_member_end);
		}

		//! Push larger elements to main chain
		main_chain.insert(main_chain.end(), sequence.begin() + right_member_start, sequence.begin() + right_member_end);
	}

	//! Push leftover elements to main chain
	pending_chain.insert(pending_chain.end(), sequence.begin() + amount_of_elements_to_pair, sequence.begin() + sequence_size);
}

void	PmergeMe::_generateJacobsthalSequence( PmergeMe::VectorSequence & sequence, int n, int c )
{
	int	prev1 = 1;
	int	prev2 = 0;

	for (int i = PmergeMe::JACOBSTHAL_START_INDEX; i <= n; ++i)
	{
		int	curr = prev1 + 2 * prev2;
		if (curr > c)
			break;
		prev2 = prev1;
		prev1 = curr;
		sequence.push_back(curr);
	}
}

void PmergeMe::_insertWithJacobsthalSequence( VectorSequence & larger_elements, VectorSequence & smaller_elements )
{
	size_t			smaller_elements_size = smaller_elements.size();
	
	VectorSequence	jacobsthal_sequence;
	this->_generateJacobsthalSequence(jacobsthal_sequence, PmergeMe::MAX_JACOBSTHAL_INDEX, smaller_elements_size);
	
	size_t			jacobsthal_sequence_size = jacobsthal_sequence.size();

	for (size_t i = 0; i < jacobsthal_sequence_size; ++i)
	{
		int	range_start_index = jacobsthal_sequence[i] - 1;
		int	range_end_index;
		if (i + 1 >= jacobsthal_sequence_size)
		{
			range_end_index = smaller_elements_size;
		}
		else
		{
			range_end_index = jacobsthal_sequence[i + 1] - 1;
		}

		for (int j = range_end_index - 1; j >= range_start_index; --j)
		{
			this->_binaryInsertion(larger_elements, smaller_elements[j]);
		}
	}
}

void PmergeMe::_binaryInsertion( VectorSequence & sequence, int number )
{
	VectorSequence::iterator	it = std::lower_bound(sequence.begin(), sequence.end(), number);

	sequence.insert(it, number);
}

// Sorting - List
void PmergeMe::_mergeInsertionSort( ListSequence & sequence )
{
	size_t	number_size = sequence.size();
	
	if (number_size == 0 || number_size == 1)
	return ;

	if (number_size == 2)
	{
		this->_swapTwoElements(sequence);
		return ;
	}

	ListSequence	larger_elements;
	ListSequence	smaller_elements;
	
	this->_splitToChains(sequence, larger_elements, smaller_elements);
	
	this->_mergeInsertionSort(larger_elements);
	
	this->_insertWithJacobsthalSequence(larger_elements, smaller_elements);

	sequence = larger_elements;
}

void PmergeMe::_swapTwoElements( ListSequence & sequence )
{
	if (sequence.front() > sequence.back())
	{
		std::swap(sequence.front(), sequence.back());
	}
}

void PmergeMe::_splitToChains( ListSequence const & sequence, ListSequence & larger_elements, ListSequence & smaller_elements )
{
	for (ListSequence::const_iterator curr_it = sequence.begin(); curr_it != sequence.end(); )
	{
		ListSequence::const_iterator	next_it = std::next(curr_it);

		if (next_it == sequence.end())
		{
			break;
		}

		int first = *curr_it;
		int second = *next_it;
		int larger = std::max(first, second);
		int smaller = std::min(first, second);

		larger_elements.push_back(larger);
		smaller_elements.push_back(smaller);

		std::advance(curr_it, 2);
	}

	if (sequence.size() % 2 != 0)
		smaller_elements.push_back(sequence.back());
}

void	PmergeMe::_generateJacobsthalSequence( PmergeMe::ListSequence & sequence, int n, int c )
{
	int	prev1 = 1;
	int	prev2 = 0;

	for (int i = PmergeMe::JACOBSTHAL_START_INDEX; i <= n; ++i)
	{
		int	curr = prev1 + 2 * prev2;
		if (curr >= c)
			break;
		prev2 = prev1;
		prev1 = curr;
		sequence.push_back(curr);
	}
}

void PmergeMe::_insertWithJacobsthalSequence( ListSequence & larger_elements, ListSequence & smaller_elements )
{
	size_t			smaller_elements_size = smaller_elements.size();

	ListSequence	jacobsthal_sequence;
	this->_generateJacobsthalSequence(jacobsthal_sequence, PmergeMe::MAX_JACOBSTHAL_INDEX, smaller_elements_size);

	size_t			jacobsthal_sequence_size = jacobsthal_sequence.size();

	for (size_t i = 0; i < jacobsthal_sequence_size; ++i)
	{
		ListSequence::iterator jac_it_start = jacobsthal_sequence.begin();
		std::advance(jac_it_start, i);
		int	range_start_index = *jac_it_start - 1;
		
		int	range_end_index;
		if (i + 1 >= jacobsthal_sequence_size)
		{
			range_end_index = smaller_elements_size;
		}
		else
		{
			ListSequence::iterator jac_it_end = jacobsthal_sequence.begin();
			std::advance(jac_it_end, i + 1);
			range_end_index = *jac_it_end - 1;
		}

		for (int j = range_end_index - 1; j >= range_start_index; --j)
		{
			ListSequence::iterator small_it = smaller_elements.begin();
			std::advance(small_it, j);
			this->_binaryInsertion(larger_elements, *small_it);
		}
	}
}

void PmergeMe::_binaryInsertion( ListSequence & sequence, int number )
{
	ListSequence::iterator	it = std::lower_bound(sequence.begin(), sequence.end(), number);

	sequence.insert(it, number);
}

// Utility helpers
void PmergeMe::_printNumbers( VectorSequence const & sequence ) const
{
	for (VectorSequence::const_iterator it = sequence.begin(); it != sequence.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl; //! REMOVE
}

void PmergeMe::_printNumbers( ListSequence const & sequence ) const
{
	for (ListSequence::const_iterator it = sequence.begin(); it != sequence.end(); ++it)
	{
		std::cout << *it << " ";
	}
}
