CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I..
TARGET = icg

# Source files for ICG
ICG_SRCS = icg_main.c icg.c

# Parser object files needed for linking (excluding main.o to avoid conflicts)
PARSER_OBJS = ../Parser-G24/parser.o ../Parser-G24/parse_tree.o

# Object files for ICG
ICG_OBJS = $(ICG_SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(ICG_OBJS) $(PARSER_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ICG_OBJS) $(PARSER_OBJS)

# Ensure parser objects are built
../Parser-G24/parser.o: ../Parser-G24/parser.c
	cd ../Parser-G24 && $(CC) $(CFLAGS) -c parser.c -o parser.o

../Parser-G24/parse_tree.o: ../Parser-G24/parse_tree.c
	cd ../Parser-G24 && $(CC) $(CFLAGS) -c parse_tree.c -o parse_tree.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) ../program.txt icg_output.txt

clean:
	rm -f $(TARGET) $(ICG_OBJS) icg_output.txt
	cd ../Parser-G24 && rm -f parser.o parse_tree.o main.o

.PHONY: all run clean
