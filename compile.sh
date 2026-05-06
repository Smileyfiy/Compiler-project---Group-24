#!/bin/bash

###############################################################################
# Unified Compiler Pipeline
# Runs program.txt through Lexer-G24, Parser-G24, then ICG-G24
# Shows outputs of all stages separately
###############################################################################

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# File paths
PROGRAM_FILE="program.txt"
LEXER_DIR="Lexer-G24"
PARSER_DIR="Parser-G24"
ICG_DIR="ICG-G24"

# Check if program.txt exists
if [ ! -f "$PROGRAM_FILE" ]; then
    echo -e "${RED}Error: $PROGRAM_FILE not found${NC}"
    echo "Please ensure program.txt exists in the current directory"
    exit 1
fi

# Check if both projects exist
if [ ! -d "$LEXER_DIR" ]; then
    echo -e "${RED}Error: $LEXER_DIR directory not found${NC}"
    exit 1
fi

if [ ! -d "$PARSER_DIR" ]; then
    echo -e "${RED}Error: $PARSER_DIR directory not found${NC}"
    exit 1
fi

if [ ! -d "$ICG_DIR" ]; then
    echo -e "${RED}Error: $ICG_DIR directory not found${NC}"
    exit 1
fi

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}     COMPILER PIPELINE ${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo ""

# ============================================================================
# STEP 1: Display Source Program
# ============================================================================
echo -e "${GREEN}STEP 1: SOURCE PROGRAM${NC}"
echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"
cat "$PROGRAM_FILE"
echo ""
echo ""

# ============================================================================
# STEP 2: Run Lexer
# ============================================================================
echo -e "${GREEN}STEP 2: LEXICAL ANALYSIS ${NC}"
echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"

# Copy program.txt to lexer directory
cp "$PROGRAM_FILE" "$LEXER_DIR/program.txt"

# Check if lexer is compiled
if [ ! -f "$LEXER_DIR/lexer" ]; then
    echo -e "${YELLOW}Compiling Lexer...${NC}"
    make -C "$LEXER_DIR" clean >/dev/null 2>&1
    make -C "$LEXER_DIR" >/dev/null 2>&1
    echo -e "${GREEN}✓ Lexer compiled${NC}"
fi

# Run lexer and capture output
echo -e "${GREEN}Running Lexer:${NC}"
cd "$LEXER_DIR"
./lexer
cd ..

echo ""
echo ""

# ============================================================================
# STEP 3: Run Parser
# ============================================================================
echo -e "${GREEN}STEP 3: SYNTAX ANALYSIS ${NC}"
echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"

# Copy program.txt to parser directory
cp "$PROGRAM_FILE" "$PARSER_DIR/program.txt"

# Check if parser is compiled
if [ ! -f "$PARSER_DIR/parser" ]; then
    echo -e "${YELLOW}Compiling Parser...${NC}"
    make -C "$PARSER_DIR" clean >/dev/null 2>&1
    make -C "$PARSER_DIR" >/dev/null 2>&1
    echo -e "${GREEN}✓ Parser compiled${NC}"
fi

# Run parser and capture output
echo -e "${GREEN}Running Parser:${NC}"
cd "$PARSER_DIR"
./parser
cd ..

echo ""
echo ""

# ============================================================================
# STEP 4: Run ICG (Intermediate Code Generation)
# ============================================================================
echo -e "${GREEN}STEP 4: INTERMEDIATE CODE GENERATION ${NC}"
echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"

cp "$PROGRAM_FILE" "$ICG_DIR/program.txt"

# Check if ICG is compiled
if [ ! -f "$ICG_DIR/icg" ]; then
    echo -e "${YELLOW}Compiling ICG...${NC}"
    make -C "$ICG_DIR" clean >/dev/null 2>&1
    make -C "$ICG_DIR" >/dev/null 2>&1
    echo -e "${GREEN}✓ ICG compiled${NC}"
fi

# Run ICG and capture output
echo -e "${GREEN}Running ICG:${NC}"
cd "$ICG_DIR"
./icg "../$PROGRAM_FILE" icg_output.txt
cd ..

echo ""
echo ""
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}✓ End of program ${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
