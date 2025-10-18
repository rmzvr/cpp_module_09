#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

class PmergeMe
{
	public:
		using	VectorSequence = std::vector<int>;
		using	ListSequence = std::list<int>;

		PmergeMe();
		~PmergeMe();

		void	sort( VectorSequence & sequence );
		void	sort( ListSequence & sequence );

		void	printNumbersVector( std::vector<int> const & sequence );

	private:
		static constexpr int MAX_JACOBSTHAL_INDEX = 33;
		static constexpr int JACOBSTHAL_START_INDEX = 3;
		static constexpr int MIN_SIZE_FOR_SORTING = 2;

		void	_mergeInsertionSort( VectorSequence & sequence );
		void	_swapTwoElements( VectorSequence & sequence );
		void	_splitToChains( VectorSequence const & sequence, VectorSequence & larger_elements, VectorSequence & smaller_elements );
		void	_binaryInsertion( VectorSequence & sequence, int number );


		void	_mergeInsertionSort( ListSequence & sequence );
		void	_swapTwoElements( ListSequence& sequence );
		void	_splitToChains( ListSequence const & sequence, VectorSequence & larger_elements, VectorSequence & smaller_elements );
		void	_binaryInsertion( ListSequence& sequence, int number );

		void			_insertWithJacobsthalSequence( VectorSequence & larger_elements, VectorSequence & smaller_elements );
		VectorSequence	_getJacobsthalSequence( int n, int c );
};