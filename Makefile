SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
INC_FILES = $(wildcard $(INC_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

EXTRAS = -g -Wno-unused-parameter -Wno-unused-variable -Wno-empty-body
SFLAGS = $(shell sdl2-config --cflags) -lSDL2_ttf
CFLAGS = $(SFLAGS) $(EXTRAS) -Wall -Wextra -Werror -pedantic
IFLAGS = -I $(INC_DIR)
LFLAGS = $(shell sdl2-config --libs) -lm

TARGETS = rayblocks

all: $(TARGETS)

rayblocks: % : $(OBJ_FILES)
	@echo -e "LINK\t$@"
	@$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@echo -e "CC\t$@"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGETS)

re:
	@$(MAKE) -s clean
	@$(MAKE) -s

.PHONY: all clean re
