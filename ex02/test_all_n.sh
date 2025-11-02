#!/bin/bash

# Path to the executable
PROGRAM="./PmergeMe"

# Check if program exists
if [ ! -f "$PROGRAM" ]; then
	echo "Error: $PROGRAM not found. Please compile first with 'make'"
	exit 1
fi

# Test function
test_n_elements() {
	local n=$1
	echo ""
	echo "Testing with n=$n elements:"
	
	# Generate n random numbers (positive integers)
	local numbers=$(shuf -i 1-10000 -n $n | tr '\n' ' ')
	
	# Run the program and capture output
	local output=$($PROGRAM $numbers 2>&1)
	
	echo "$output"
	echo "----------------------------------------"
}

echo "=== Testing PmergeMe with various n values ==="

# Test edge cases
echo ""
echo "=== Edge Cases ==="
test_n_elements 1
test_n_elements 2
test_n_elements 3

# Test small values (n=4 to n=20)
echo ""
echo "=== Small values (n=4 to n=20) ==="
for n in {4..20}; do
	test_n_elements $n
done

# Test medium values (n=21 to n=33)
echo ""
echo "=== Medium values (n=21 to n=33) ==="
for n in {21..33}; do
	test_n_elements $n
done

# Test larger values
echo ""
echo "=== Larger values ==="
for n in 50 100 200 500 1000 3000; do
	test_n_elements $n
done

echo ""
echo "=== All tests completed ==="
