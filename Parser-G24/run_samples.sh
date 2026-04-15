#!/bin/bash

# Sample Test Runner Script for LL(1) Parser

echo "=========================================="
echo "LL(1) Parser - Sample Test Suite"
echo "=========================================="
echo ""

# Test 1: Simple Declarations
echo "TEST 1: Simple Declarations"
echo "---"
cp samples/sample1_declarations.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
echo ""

# Test 2: Expressions
echo "TEST 2: Arithmetic Expressions"
echo "---"
cp samples/sample2_expressions.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
echo ""

# Test 3: If Statement
echo "TEST 3: If Statement"
echo "---"
cp samples/sample3_if_statement.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
echo ""

# Test 4: While Loop
echo "TEST 4: While Loop"
echo "---"
cp samples/sample4_while_loop.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
echo ""

# Test 5: For Loop
echo "TEST 5: For Loop"
echo "---"
cp samples/sample5_for_loop.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
echo ""

# Test 6: Complex Program
echo "TEST 6: Complex Program (All Features)"
echo "---"
cp samples/sample6_complex.txt program.txt
echo "Input:"
cat program.txt
echo ""
echo "Output:"
./parser
echo ""
echo "=========================================="
