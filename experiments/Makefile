CC = gcc
CFLAGS = -Wall -O2 -pthread
TARGET = experiments
OBJS = experiments.o funcs.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

experiments.o: experiments.c funcs.h
	$(CC) $(CFLAGS) -c experiments.c

funcs.o: funcs.c funcs.h
	$(CC) $(CFLAGS) -c funcs.c

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
