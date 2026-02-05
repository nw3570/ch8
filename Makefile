SRC_DIR  := src
OBJ_DIR  := obj
TARGET   := ch8

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

CC       := gcc
INCLUDES := -Iinclude
CFLAGS   := -Wall -Wextra
LDFLAGS  := -lncurses

#CFLAGS_DEBUG   := $(CFLAGS_COMMON) -g -O0
#CFLAGS_RELEASE := $(CFLAGS_COMMON) -O2

all: $(TARGET)

clean: 
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.PHONY: all clean
