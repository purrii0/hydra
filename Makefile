CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/*.c
TARGET = hydra

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

