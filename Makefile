CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g
TARGET = tinyshell
SRCS = main.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
