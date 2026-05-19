CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
SRC = src/datapath.c
OBJ = $(SRC:.c=.o)
TARGET = mock_datapath

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o $(TARGET)
