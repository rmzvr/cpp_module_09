#include "PmergeMe.hpp"

// Initialize static members
int PmergeMe::_comparisons = 0;
std::unordered_map<int, int> PmergeMe::_expectedComparisons = {
	{1, 0}, {2, 1}, {3, 3}, {4, 5}, {5, 7}, {6, 10}, {7, 13}, {8, 16}, {9, 19}, {10, 22},
	{11, 26}, {12, 30}, {13, 34}, {14, 38}, {15, 42}, {16, 46}, {17, 50}, {18, 54}, {19, 58}, {20, 62},
	{21, 66}, {22, 71}, {23, 76}, {24, 81}, {25, 86}, {26, 91}, {27, 96}, {28, 101}, {29, 106}, {30, 111},
	{31, 116}, {32, 121}, {33, 126}
};

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

int PmergeMe::getExpectedComparisons( int n )
{
	auto it = _expectedComparisons.find(n);
	if (it != _expectedComparisons.end())
		return it->second;
	
	// For n > 33, we can use the theoretical upper bound
	// F(n) ≤ n⌈lg n⌉ - 2^⌈lg n⌉ + 1
	if (n > 33)
	{
		int	lg_n = static_cast<int>(std::ceil(std::log2(n)));
		return n * lg_n - (1 << lg_n) + 1;
	}
	
	return -1;
}

bool PmergeMe::validateComparisons( int n, int actualComparisons )
{
	int const	expected = getExpectedComparisons(n);

	if (expected == -1)
	{
		return true;
	}

	return actualComparisons <= expected;
}

void PmergeMe::resetComparisons()
{
	_comparisons = 0;
}

void PmergeMe::incrementComparisons()
{
	_comparisons++;
}

int PmergeMe::getComparisons()
{
	return _comparisons;
}

// Sorting - Vector

void	PmergeMe::_mergeInsertionSort( VectorSequence & sequence, size_t sequence_size, size_t group_level )
{
	size_t const	pair_size = 1 << group_level;
	size_t const	member_size = pair_size / 2;
	size_t const	pair_count = sequence_size / pair_size;
	size_t const	paired_elements = pair_count * pair_size;

	if (pair_count < 1)
	{
		return;
	}

	#ifdef DEBUG
		std::cout << "\ngroup_level - " << group_level;
		std::cout << "\npair_size - " << pair_size;
		std::cout << "\namount_elements_in_member - " << member_size;
		std::cout << "\npair_count - " << pair_count;
		std::cout << "\npaired_elements - " << paired_elements;
		std::cout << "\nsequence_size - " << sequence_size;
		std::cout << "\n" << std::endl;

		this->printSequence(sequence, "Sequence before sorting pairs");

		std::cout << "Sorting pairs..." << std::endl;
	#endif

	this->_sortPairs(sequence, pair_size, member_size, paired_elements);

	#ifdef DEBUG
		this->printSequence(sequence, "Sequence after sorting pairs");
	#endif

	if (pair_count > 1)
	{
		#ifdef DEBUG
			std::cout << "\nEnter recursion [" << group_level + 1 << "]..." << std::endl;
		#endif

		this->_mergeInsertionSort(sequence, paired_elements, group_level + 1);

		#ifdef DEBUG
			std::cout << "\nExit recursion [" << group_level + 1 << "]..." << std::endl;
		#endif
	}

	if (pair_count < 2 && paired_elements == sequence_size)
	{
		return;
	}

	VectorSequence	main_chain;
	VectorSequence	pending_chain;

	//? Optimize chains reallocations from O(n log n) to O(n)
	main_chain.reserve(sequence_size);
	pending_chain.reserve(sequence_size);

	#ifdef DEBUG
		this->printSequence(sequence, "Sequence before splitting to chains");
		std::cout << "sequence_size - " << sequence_size << std::endl;

		std::cout << "\nSplit sequence to chains..." << std::endl;
	#endif

	this->_splitToChains(sequence, main_chain, pending_chain, pair_size, member_size, paired_elements, sequence_size);

	#ifdef DEBUG
		this->printSequence(main_chain, "\nMain chain after splitting");
		this->printSequence(pending_chain, "Pending chain after splitting");

		std::cout << "\nInserting pending chain to main chain..." << std::endl;
	#endif

	this->_insertPendingGroupsWithJacobsthal(sequence, main_chain, pending_chain, member_size);
}

void	PmergeMe::_sortPairs( VectorSequence & sequence, size_t pair_size, size_t member_size, size_t paired_elements)
{
	for (
		size_t	i = 0;
		i < paired_elements;
		i += pair_size
	)
	{
		size_t const	left_member_start = i;
		size_t const	left_member_end = left_member_start + member_size;
		size_t const	right_member_end = left_member_end + member_size;

		if (sequence[left_member_end - 1] > sequence[right_member_end - 1])
		{
			std::swap_ranges(
				sequence.begin() + left_member_start,
				sequence.begin() + left_member_end,
				sequence.begin() + left_member_end
			);
		}
		PmergeMe::incrementComparisons();
	}
}

void	PmergeMe::_splitToChains(VectorSequence const & sequence, VectorSequence & main_chain, VectorSequence & pending_chain, size_t pair_size, size_t member_size, size_t paired_elements, size_t sequence_size)
{
	for (size_t	i = 0; i < paired_elements; i += pair_size)
	{
		size_t const	left_member_start = i;
		size_t const	left_member_end = left_member_start + member_size;
		size_t const	right_member_start = left_member_end;
		size_t const	right_member_end = right_member_start + member_size;

		if (i == 0)
		{
			//! Push first smaller elements to main chain
			main_chain.insert(main_chain.end(), sequence.begin() + left_member_start, sequence.begin() + left_member_end);
		}
		else
		{
			//! Push rest smaller elements to pending chain
			pending_chain.insert(pending_chain.end(), sequence.begin() + left_member_start, sequence.begin() + left_member_end);
		}

		//! Push larger elements to main chain
		main_chain.insert(main_chain.end(), sequence.begin() + right_member_start, sequence.begin() + right_member_end);
	}

	//! Push leftover elements to main chain
	pending_chain.insert(pending_chain.end(), sequence.begin() + paired_elements, sequence.begin() + sequence_size);
}

void	PmergeMe::_insertPendingGroupsWithJacobsthal( VectorSequence & sequence, VectorSequence & main_chain, VectorSequence & pending_chain, size_t member_size )
{
	VectorSequence	jacobsthal_sequence;
	size_t const	pending_chain_members_amount = pending_chain.size() / member_size;

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
			this->_insertPendingGroup(main_chain, pending_chain, i, member_size, group_index);
		}
		group_index++;
	}

	#ifdef DEBUG
		this->printSequence(main_chain, "\nMain chain after insertion");
		this->printSequence(pending_chain, "Pending chain after insertion");

		this->printSequence(sequence, "\nSequence before main chain copy");
	#endif

	std::copy(main_chain.begin(), main_chain.end(), sequence.begin());

	#ifdef DEBUG
		this->printSequence(sequence, "\nSequence after main chain copy");
	#endif
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

void	PmergeMe::_insertPendingGroup( VectorSequence & main_chain, VectorSequence const & pending_chain, int i, size_t member_size, size_t group_index )
{
	size_t const	group_start = i * member_size;
	size_t const	group_end = group_start + member_size;

	size_t const	search_limit = (std::pow(2, group_index) - 1) * member_size;
	size_t const	max_search_range = std::min(main_chain.size(), search_limit);

	size_t const	largest_element_in_group = pending_chain[group_end - 1];
	size_t const	idx_to_insert = this->_findGroupInsertionIndex(main_chain, max_search_range, largest_element_in_group, member_size);

	main_chain.insert(main_chain.begin() + idx_to_insert, pending_chain.begin() + group_start, pending_chain.begin() + group_end);
}

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

		PmergeMe::incrementComparisons();
	}

	return left * elements_per_group;
}

// Sorting - List

void	PmergeMe::_mergeInsertionSort( ListSequence & sequence, size_t sequence_size, size_t group_level )
{
	size_t const	pair_size = 1 << group_level;
	size_t const	member_size = pair_size / 2;
	size_t const	pair_count = sequence_size / pair_size;
	size_t const	paired_elements = pair_count * pair_size;

	if (pair_count < 1)
	{
		return;
	}

	#ifdef DEBUG
		std::cout << "\ngroup_level - " << group_level;
		std::cout << "\npair_size - " << pair_size;
		std::cout << "\namount_elements_in_member - " << member_size;
		std::cout << "\npair_count - " << pair_count;
		std::cout << "\npaired_elements - " << paired_elements;
		std::cout << "\nsequence_size - " << sequence_size;
		std::cout << "\n" << std::endl;

		this->printSequence(sequence, "Sequence before sorting pairs");

		std::cout << "Sorting pairs..." << std::endl;
	#endif

	this->_sortPairs(sequence, pair_size, member_size, paired_elements);

	#ifdef DEBUG
		this->printSequence(sequence, "Sequence after sorting pairs");
	#endif

	if (pair_count > 1)
	{
		#ifdef DEBUG
			std::cout << "\nEnter recursion [" << group_level + 1 << "]..." << std::endl;
		#endif

		this->_mergeInsertionSort(sequence, paired_elements, group_level + 1);

		#ifdef DEBUG
			std::cout << "\nExit recursion [" << group_level + 1 << "]..." << std::endl;
		#endif
	}

	if (pair_count < 2 && paired_elements == sequence_size)
	{
		return;
	}

	ListSequence	main_chain;
	ListSequence	pending_chain;

	#ifdef DEBUG
		this->printSequence(sequence, "Sequence before splitting to chains");
		std::cout << "sequence_size - " << sequence_size << std::endl;

		std::cout << "\nSplit sequence to chains..." << std::endl;
	#endif

	this->_splitToChains(sequence, main_chain, pending_chain, pair_size, member_size, paired_elements, sequence_size);

	#ifdef DEBUG
		this->printSequence(main_chain, "\nMain chain after splitting");
		this->printSequence(pending_chain, "Pending chain after splitting");

		std::cout << "\nInserting pending chain to main chain..." << std::endl;
	#endif

	this->_insertPendingGroupsWithJacobsthal(sequence, main_chain, pending_chain, member_size);
}

void	PmergeMe::_sortPairs( ListSequence & sequence, size_t pair_size, size_t member_size, size_t paired_elements )
{
	auto	current = sequence.begin();

	for (size_t	i = 0; i < paired_elements; i += pair_size)
	{
		auto	last_left_members_element = std::next(current, member_size - 1);
		auto	last_right_members_element = std::next(last_left_members_element, member_size);

		if (*last_left_members_element > *last_right_members_element)
		{
			auto	left_member_start = current;
			auto	right_member_start = std::next(left_member_start, member_size);

			for (size_t j = 0; j < member_size; ++j)
			{
				std::iter_swap(left_member_start++, right_member_start++);
			}
		}
		PmergeMe::incrementComparisons();
		std::advance(current, pair_size);
	}
}

void	PmergeMe::_splitToChains( ListSequence const & sequence, ListSequence & main_chain, ListSequence & pending_chain, size_t pair_size, size_t member_size, size_t paired_elements, size_t sequence_size )
{
	auto	current = sequence.begin();

	for (size_t	i = 0; i < paired_elements; i += pair_size)
	{
		auto	left_member_start = current;
		auto	left_member_end = std::next(current, member_size);
		auto	right_member_start = left_member_end;
		auto	right_member_end = std::next(right_member_start, member_size);

		if (i == 0)
		{
			//! Push first smaller elements to main chain
			main_chain.insert(main_chain.end(), left_member_start, left_member_end);
		}
		else
		{
			//! Push rest smaller elements to pending chain
			pending_chain.insert(pending_chain.end(), left_member_start, left_member_end);
		}

		//! Push larger elements to main chain
		main_chain.insert(main_chain.end(), right_member_start, right_member_end);
		
		current = right_member_end;
	}

	//! Push leftover elements to pending chain
	if (paired_elements < sequence_size)
	{
		auto	leftover_start = std::next(sequence.begin(), paired_elements);
		pending_chain.insert(pending_chain.end(), leftover_start, std::next(sequence.begin(), sequence_size));
	}
}

void	PmergeMe::_insertPendingGroupsWithJacobsthal( ListSequence & sequence, ListSequence & main_chain, ListSequence & pending_chain, size_t member_size )
{
	ListSequence	jacobsthal_sequence;
	size_t const	pending_chain_members_amount = pending_chain.size() / member_size;

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
			this->_insertPendingGroup(main_chain, pending_chain, i, member_size, group_index);
		}
		group_index++;
	}

	#ifdef DEBUG
		this->printSequence(main_chain, "\nMain chain after insertion");
		this->printSequence(pending_chain, "Pending chain after insertion");

		this->printSequence(sequence, "\nSequence before main chain assign");

		this->printSequence(main_chain, "Main chain before assign to sequence");
	#endif

	auto seq_it = sequence.begin();
	auto main_it = main_chain.begin();
	while (main_it != main_chain.end() && seq_it != sequence.end())
	{
		*seq_it = *main_it;
		++seq_it;
		++main_it;
	}

	#ifdef DEBUG
		this->printSequence(sequence, "\nSequence after main chain assign");
	#endif
}

void	PmergeMe::_generateJacobsthalSequence( PmergeMe::ListSequence & sequence, int n, int c )
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

void	PmergeMe::_insertPendingGroup( ListSequence & main_chain, ListSequence const & pending_chain, int i, size_t member_size, size_t group_index )
{
	auto			group_start = std::next(pending_chain.begin(), i * member_size);
	auto			group_end = std::next(group_start, member_size);

	size_t const	search_limit = (std::pow(2, group_index) - 1) * member_size;
	size_t const	max_search_range = std::min(main_chain.size(), search_limit);

	size_t const	largest_element_in_group = *std::prev(group_end);
	size_t const	index_to_insert = this->_findGroupInsertionIndex(main_chain, max_search_range, largest_element_in_group, member_size);

	auto			insertion_position = std::next(main_chain.begin(), index_to_insert);
	main_chain.insert(insertion_position, group_start, group_end);
}

size_t	PmergeMe::_findGroupInsertionIndex( ListSequence const & chain, size_t max_search_range, int target_value, size_t elements_per_group )
{
	int	left = 0;
	int	right = max_search_range / elements_per_group;

	while (left < right)
	{
		int		middle_idx = left + (right - left) / 2;
		auto	middle_it = std::next(chain.begin(), middle_idx * elements_per_group + elements_per_group - 1);
		int		middle_value = *middle_it;

		if (middle_value < target_value)
			left = middle_idx + 1;
		else
			right = middle_idx;

		PmergeMe::incrementComparisons();
	}

	return left * elements_per_group;
}
