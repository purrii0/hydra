CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude -g

SRC     = main.c $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)
TARGET  = hydra

.PHONY: all build clean

all: build

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

