# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g `sdl2-config --cflags` -lSDL2_image
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

# Source and object files
SRC = main.c player.c monster.c projectile.c comet.c game.c
OBJ = $(SRC:.c=.o)
TARGET = game

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJ) $(TARGET)
