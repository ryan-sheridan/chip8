CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc -I/opt/homebrew/include `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`
OBJ_FILES = main.o src/sound.o src/graphics.o src/chip8.o src/debug.o src/ins.o
EXEC = chip8_emulator

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC) $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXEC)
	rm -f *~
