# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Executable name
TARGET = lexer

# Source file
SRC = lexer.c

# Default rule
all: $(TARGET)

# Compile the lexer
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Run the lexer
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET) *.o