#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Path to the executable
PROGRAM="./PmergeMe"

# Check if program exists
if [ ! -f "$PROGRAM" ]; then
    echo -e "${RED}Error: $PROGRAM not found. Please compile first with 'make'${NC}"
    exit 1
fi

# Test function
test_n_elements() {
    local n=$1
    echo -e "\n${YELLOW}Testing with n=$n elements:${NC}"
    
    # Generate n random numbers (positive integers)
    local numbers=$(shuf -i 1-10000 -n $n | tr '\n' ' ')
    
    # echo "Input: $numbers"
    
    # Run the program
    $PROGRAM $numbers
    
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✓ Test passed for n=$n${NC}"
    else
        echo -e "${RED}✗ Test failed for n=$n (exit code: $exit_code)${NC}"
    fi
    
    echo "----------------------------------------"
}

echo -e "${YELLOW}=== Testing PmergeMe with various n values ===${NC}"

# Test edge cases
echo -e "\n${YELLOW}=== Edge Cases ===${NC}"
test_n_elements 1
test_n_elements 2
test_n_elements 3

# Test small values (n=4 to n=20)
echo -e "\n${YELLOW}=== Small values (n=4 to n=20) ===${NC}"
for n in {4..20}; do
    test_n_elements $n
done

# Test medium values (n=21 to n=33)
echo -e "\n${YELLOW}=== Medium values (n=21 to n=33) ===${NC}"
for n in {21..33}; do
    test_n_elements $n
done

# Test larger values
echo -e "\n${YELLOW}=== Larger values ===${NC}"
for n in 50 100 200 500 1000 3000; do
    test_n_elements $n
done

echo -e "\n${GREEN}=== All tests completed ===${NC}"
