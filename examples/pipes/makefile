CC = gcc

CFLAGS = -Wall -Wextra -I../../include
LDFLAGS = -L../../lib/local -lesclib -lm

TARGET = pipes
SRC = $(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
