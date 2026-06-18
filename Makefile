CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/calc.c src/ui.c src/parser.c
TARGET = calculator

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	-del /Q $(TARGET).exe 2>nul
	@echo clean done
