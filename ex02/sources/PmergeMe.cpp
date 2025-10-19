#include "PmergeMe.hpp"

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
	this->_mergeInsertionSort(sequence);
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
	std::cout << std::endl;
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

void PmergeMe::_mergeInsertionSort( VectorSequence & sequence )
{
	size_t	number_size = sequence.size();
	
	if (number_size == 0 || number_size == 1)
	return ;

	if (number_size == PmergeMe::MIN_SIZE_FOR_SORTING)
	{
		this->_swapTwoElements(sequence);
		return ;
	}

	VectorSequence	larger_elements;
	VectorSequence	smaller_elements;
	
	this->_splitToChains(sequence, larger_elements, smaller_elements);
	
	this->_mergeInsertionSort(larger_elements);
	
	this->_insertWithJacobsthalSequence(larger_elements, smaller_elements);
	
	sequence = larger_elements;
}

void PmergeMe::_swapTwoElements( VectorSequence & sequence )
{
	if (sequence[0] > sequence[1])
	{
		std::swap(sequence[0], sequence[1]);
	}
}

void PmergeMe::_splitToChains( VectorSequence const & sequence, VectorSequence & larger_elements, VectorSequence & smaller_elements )
{
	size_t	pairs = sequence.size() / 2;
	larger_elements.reserve(pairs);
	smaller_elements.reserve(pairs + (sequence.size() % 2));

	for (VectorSequence::const_iterator curr_it = sequence.begin(); curr_it < sequence.end(); curr_it += 2)
	{
		VectorSequence::const_iterator	next_it = std::next(curr_it);

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
	}

	if (sequence.size() % 2 != 0)
		smaller_elements.push_back(sequence.back());
}

void	PmergeMe::_generateJacobsthalSequence( PmergeMe::VectorSequence & sequence, int n, int c )
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
}

void PmergeMe::_printNumbers( ListSequence const & sequence ) const
{
	for (ListSequence::const_iterator it = sequence.begin(); it != sequence.end(); ++it)
	{
		std::cout << *it << " ";
	}
}
