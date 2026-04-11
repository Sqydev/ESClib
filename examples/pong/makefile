CC = gcc

CFLAGS = -Wall -Wextra -I../../include
LDFLAGS = -L../../lib/local -lesclib -lm -Wl,-rpath=../../lib/local

TARGET = pong
SRC = $(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
