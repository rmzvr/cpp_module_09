#pragma once

#include <list>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_map>

class PmergeMe
{
	public:
		using	VectorSequence = std::vector<int>;
		using	ListSequence = std::list<int>;

		PmergeMe();
		PmergeMe( PmergeMe const & other );
		PmergeMe &	operator=( PmergeMe const & other );
		~PmergeMe();

		void	sort( VectorSequence & sequence );
		void	sort( ListSequence & sequence );

		// Utility functions - Vector
		void	printSequence( VectorSequence const & sequence, std::string const & label ) const;
		void	printTime( VectorSequence const & sequence, std::chrono::duration<double, std::micro> const & duration, std::string const & containerName ) const;
		bool	verifySorted( VectorSequence const & sequence ) const;
		std::chrono::duration<double, std::micro>	sortAndMeasureTime( VectorSequence & sequence );

		// Utility functions - List
		void	printSequence( ListSequence const & sequence, std::string const & label ) const;
		void	printTime( ListSequence const & sequence, std::chrono::duration<double, std::micro> const & duration, std::string const & containerName ) const;
		bool	verifySorted( ListSequence const & sequence ) const;
		std::chrono::duration<double, std::micro>	sortAndMeasureTime( ListSequence & sequence );

		// Static utility
		static int	parseArgument( char *argument );
		static int	getExpectedComparisons( int n );
		static bool	validateComparisons( int n, int actualComparisons );

		// Comparison tracking
		static void	resetComparisons();
		static void	incrementComparisons();
		static int	getComparisons();

	private:
		static constexpr int	MAX_JACOBSTHAL_INDEX = 33;
		static constexpr int	JACOBSTHAL_START_INDEX = 3;
		static constexpr int	MIN_SIZE_FOR_SORTING = 2;
		
		// Comparison tracking
		static int	_comparisons;
		static std::unordered_map<int, int>	_expectedComparisons;

		// Sorting - Vector
		void	_mergeInsertionSort( VectorSequence & sequence, size_t sequence_size, size_t group_level );
		void	_sortPairs( VectorSequence & sequence, size_t amount_of_members_in_pair, size_t amount_of_elements_in_member, size_t amount_of_elements_to_pair );
		void	_splitToChains( VectorSequence const & sequence, VectorSequence & main_chain, VectorSequence & pending_chain, size_t amount_of_members_in_pair, size_t amount_of_elements_in_member, size_t amount_of_elements_to_pair, size_t sequence_size );
		void	_insertPendingGroupsWithJacobsthal( VectorSequence & sequence, VectorSequence & main_chain, VectorSequence & pending_chain, size_t amount_elements_in_member );
		void	_generateJacobsthalSequence( VectorSequence & sequence, int n, int c );
		void	_insertPendingGroup( VectorSequence & main_chain, VectorSequence const & pending_chain, int group_index, size_t group_size, size_t search_depth );
		size_t	_findGroupInsertionIndex( VectorSequence const & chain, size_t max_search_range, int target_value, size_t elements_per_group );

		// Sorting - List
		void	_mergeInsertionSort( ListSequence & sequence, size_t sequence_size, size_t group_level );
		void	_sortPairs( ListSequence & sequence, size_t amount_of_members_in_pair, size_t amount_of_elements_in_member, size_t amount_of_elements_to_pair );
		void	_splitToChains( ListSequence const & sequence, ListSequence & main_chain, ListSequence & pending_chain, size_t amount_of_members_in_pair, size_t amount_of_elements_in_member, size_t amount_of_elements_to_pair, size_t sequence_size );
		void	_insertPendingGroupsWithJacobsthal( ListSequence & sequence, ListSequence & main_chain, ListSequence & pending_chain, size_t amount_elements_in_member );
		void	_generateJacobsthalSequence( ListSequence & sequence, int n, int c );
		void	_insertPendingGroup( ListSequence & main_chain, ListSequence const & pending_chain, int group_index, size_t group_size, size_t search_depth );
		size_t	_findGroupInsertionIndex( ListSequence const & chain, size_t max_search_range, int target_value, size_t elements_per_group );

		// Utility helpers
		void	_printNumbers( VectorSequence const & sequence ) const;
		void	_printNumbers( ListSequence const & sequence ) const;

};