#!/bin/bash

# Colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
RESET="\033[0m"

TEST_FILE="/Users/atomboy/42Toybox/0-Cursus/33-modularshell/tests/tsunami_testcases"
MINISHELL="/Users/atomboy/42Toybox/0-Cursus/33-modularshell/minishell"
OUTPUT_DIR="/Users/atomboy/42Toybox/0-Cursus/33-modularshell/tests/out"
PLAYGROUND_DIR="/Users/atomboy/42Toybox/0-Cursus/33-modularshell/playground"
TMP_DIR="$PLAYGROUND_DIR/tmp"
LEAK_CHECK=0

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/minishell"
mkdir -p "$OUTPUT_DIR/minishell_filtered"
mkdir -p "$OUTPUT_DIR/bash"
mkdir -p "$OUTPUT_DIR/diff"

# Create playground directory for tests
mkdir -p "$TMP_DIR"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -l|--leak-check)
            LEAK_CHECK=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [-l|--leak-check]"
            exit 1
            ;;
    esac
done

echo -e "${BLUE}=== Minishell vs Bash Comparison Test ===${RESET}"
echo -e "${YELLOW}Test started at: $(date)${RESET}"
echo

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0
leaked_tests=0
failed_indices=()
leaked_indices=()

# Clean previous test outputs
rm -f "$OUTPUT_DIR/minishell/"*
rm -f "$OUTPUT_DIR/minishell_filtered/"*
rm -f "$OUTPUT_DIR/bash/"*
rm -f "$OUTPUT_DIR/diff/"*

# Clean and prepare playground directory
rm -rf "$TMP_DIR"/*
echo -e "${BLUE}=== Prepared playground directory for tests ===${RESET}"

# テストケース読み込み
while IFS= read -r line || [ -n "$line" ]; do
    # Skip empty lines
    if [[ -z "$line" ]]; then
        continue
    fi
    
    echo -e "${YELLOW}[$total_tests] Testing:${RESET} $line"
    
    # Save test command to file for reference
    echo "$line" > "$OUTPUT_DIR/diff/$total_tests.cmd"
    
    # Clean the tmp directory before running bash
    rm -rf "$TMP_DIR"/*
    
    # Run command in bash and save output and exit status
    bash_output_file="$OUTPUT_DIR/bash/$total_tests.out"
    bash_error_file="$OUTPUT_DIR/bash/$total_tests.err"
    # Use tmp_dir as working directory
    (cd "$TMP_DIR" && echo -e "$line\nexit" | bash > "$bash_output_file" 2> "$bash_error_file")
    bash_status=$?
    echo "$bash_status" > "$OUTPUT_DIR/bash/$total_tests.status"
    
    # Run command in minishell and save output and exit status
    minishell_output_file="$OUTPUT_DIR/minishell/$total_tests.out"
    minishell_filtered_file="$OUTPUT_DIR/minishell_filtered/$total_tests.out"
    minishell_error_file="$OUTPUT_DIR/minishell/$total_tests.err"
    
    # Clean the tmp directory before running minishell
    rm -rf "$TMP_DIR"/*
    
    if [ $LEAK_CHECK -eq 1 ]; then
        # Run with valgrind if leak check is enabled
        # Use tmp_dir as working directory
        (cd "$TMP_DIR" && echo -e "$line\nexit" | valgrind --leak-check=full --show-leak-kinds=definite --error-exitcode=44 "$MINISHELL" > "$minishell_output_file" 2> "$minishell_error_file")
        minishell_status=$?
        
        # Check for memory leaks
        if [[ $minishell_status -eq 44 ]]; then
            echo -e "${RED}❌ Memory leak detected${RESET}"
            leaked_tests=$((leaked_tests + 1))
            leaked_indices+=("$total_tests")
        fi
    else
        # Run without valgrind
        # Use tmp_dir as working directory
        (cd "$TMP_DIR" && echo -e "$line\nexit" | "$MINISHELL" > "$minishell_output_file" 2> "$minishell_error_file")
        minishell_status=$?
    fi
    
    echo "$minishell_status" > "$OUTPUT_DIR/minishell/$total_tests.status"
    
    # Filter out minishell prompt for comparison
    grep -v '^minishell\$' "$minishell_output_file" > "$minishell_filtered_file"
    
    # Compare outputs
    diff_output=$(diff -u "$bash_output_file" "$minishell_filtered_file")
    diff_status=$?
    
    # Compare exit status
    bash_exit=$(cat "$OUTPUT_DIR/bash/$total_tests.status")
    minishell_exit=$(cat "$OUTPUT_DIR/minishell/$total_tests.status")
    
    # Save diff output
    if [ $diff_status -ne 0 ] || [ "$bash_exit" != "$minishell_exit" ]; then
        echo "$diff_output" > "$OUTPUT_DIR/diff/$total_tests.diff"
        echo "Bash exit status: $bash_exit" >> "$OUTPUT_DIR/diff/$total_tests.diff"
        echo "Minishell exit status: $minishell_exit" >> "$OUTPUT_DIR/diff/$total_tests.diff"
        echo -e "${RED}❌ Test failed${RESET}"
        failed_tests=$((failed_tests + 1))
        failed_indices+=("$total_tests")
    else
        echo -e "${GREEN}✅ Test passed${RESET}"
        passed_tests=$((passed_tests + 1))
    fi
    
    echo "-------------------------------------"
    total_tests=$((total_tests + 1))
done < "$TEST_FILE"

# Print summary
echo
echo -e "${BLUE}=== Test Summary ===${RESET}"
echo -e "Total tests: $total_tests"
echo -e "${GREEN}Passed: $passed_tests${RESET}"
echo -e "${RED}Failed: $failed_tests${RESET}"

if [ $LEAK_CHECK -eq 1 ]; then
    echo -e "${RED}Memory leaks: $leaked_tests${RESET}"
fi

# Print failed test indices
if [ ${#failed_indices[@]} -gt 0 ]; then
    echo
    echo -e "${RED}Failed tests (indices):${RESET}"
    for idx in "${failed_indices[@]}"; do
        cmd=$(cat "$OUTPUT_DIR/diff/$idx.cmd")
        echo -e "${RED}[$idx]${RESET} $cmd"
    done
    echo
    echo -e "${YELLOW}To view diff for a specific test:${RESET}"
    echo -e "cat $OUTPUT_DIR/diff/[test_index].diff"
    echo
    echo -e "${YELLOW}To view filtered minishell output:${RESET}"
    echo -e "cat $OUTPUT_DIR/minishell_filtered/[test_index].out"
    echo
    echo -e "${YELLOW}To view bash output:${RESET}"
    echo -e "cat $OUTPUT_DIR/bash/[test_index].out"
fi

# Print leaked test indices
if [ $LEAK_CHECK -eq 1 ] && [ ${#leaked_indices[@]} -gt 0 ]; then
    echo
    echo -e "${RED}Tests with memory leaks (indices):${RESET}"
    for idx in "${leaked_indices[@]}"; do
        cmd=$(cat "$OUTPUT_DIR/diff/$idx.cmd")
        echo -e "${RED}[$idx]${RESET} $cmd"
    done
fi

echo
echo -e "${YELLOW}Test completed at: $(date)${RESET}"

# Clean up playground tmp directory
rm -rf "$TMP_DIR"
# Exit with failure if any tests failed
if [ $failed_tests -gt 0 ] || [ $leaked_tests -gt 0 ]; then
    exit 1
fi

exit 0
