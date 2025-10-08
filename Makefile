# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Target executable
TARGET = approximation

# Source files
SOURCES = permute.c approximation.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile permute.c first
permute.o: permute.c
	$(CC) $(CFLAGS) -c permute.c

# Compile approximation.c (depends on permute.o)
approximation.o: approximation.c permute.o
	$(CC) $(CFLAGS) -c approximation.c

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild