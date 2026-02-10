SRC_DIRS  := src src/core src/ui
OBJ_DIR   := obj
TARGET    := ch8

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

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

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.PHONY: all clean
