CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc
OBJ_FILES = main.o src/sound.o src/graphics.o src/chip8.o
EXEC = chip8_emulator # Or whatever you want to call your executable

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC)

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXEC)
	rm -f *~
