CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
TARGET = matrix_lab
SOURCES = main.c matrix.c field_info.c complex.c tests.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -lm

.PHONY: clean
clean:
	rm -f $(TARGET)
