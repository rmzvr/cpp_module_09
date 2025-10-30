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
    
    # Run the program and capture output
    local output=$($PROGRAM $numbers 2>&1)
    local exit_code=$?
    
    echo "$output"
    
    if [ $exit_code -eq 0 ]; then
        # Check if output contains "PASSED"
        if echo "$output" | grep -q "Test result: PASSED"; then
            echo -e "${GREEN}✓ Test passed for n=$n${NC}"
        elif echo "$output" | grep -q "Test result: FAILED"; then
            echo -e "${RED}✗ Test failed for n=$n - Too many comparisons!${NC}"
            return 1
        else
            echo -e "${YELLOW}? Test completed for n=$n but no clear result found${NC}"
        fi
    else
        echo -e "${RED}✗ Test failed for n=$n (exit code: $exit_code)${NC}"
        return 1
    fi
    
    echo "----------------------------------------"
    return 0
}

echo -e "${YELLOW}=== Testing PmergeMe with various n values ===${NC}"

# Test counters
total_tests=0
passed_tests=0
failed_tests=0

# Helper function to run test and update counters
run_test() {
    local n=$1
    ((total_tests++))
    if test_n_elements $n; then
        ((passed_tests++))
    else
        ((failed_tests++))
    fi
}

# Test edge cases
echo -e "\n${YELLOW}=== Edge Cases ===${NC}"
run_test 1
run_test 2
run_test 3

# Test small values (n=4 to n=20)
echo -e "\n${YELLOW}=== Small values (n=4 to n=20) ===${NC}"
for n in {4..20}; do
    run_test $n
done

# Test medium values (n=21 to n=33)
echo -e "\n${YELLOW}=== Medium values (n=21 to n=33) ===${NC}"
for n in {21..33}; do
    run_test $n
done

# Test larger values
echo -e "\n${YELLOW}=== Larger values ===${NC}"
for n in 50 100 200 500 1000 3000; do
    run_test $n
done

echo -e "\n${GREEN}=== All tests completed ===${NC}"
echo -e "\n${YELLOW}Summary:${NC}"
echo -e "Total tests: $total_tests"
echo -e "${GREEN}Passed: $passed_tests${NC}"
echo -e "${RED}Failed: $failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All tests passed! The algorithm meets the Ford-Johnson comparison requirements.${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. The algorithm exceeded the expected number of comparisons.${NC}"
    exit 1
fi
