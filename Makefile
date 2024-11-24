CC = gcc
CFLAGS = -Wall -Wextra -O2 -fopenmp
LDFLAGS = -lm

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = neural_sim

.PHONY: all clean test

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test: all
	for test in tests/test_* ; do \
		$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $$test -o $(BUILD_DIR)/`basename $$test .c` $(LDFLAGS) ; \
		./$(BUILD_DIR)/`basename $$test .c` ; \
	done
