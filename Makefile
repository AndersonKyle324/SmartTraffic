CC = g++
CFLAGS = -Wall -Iinclude
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Find all .cpp and .c files in the src directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.c)

# Generate corresponding .o paths in the build directory
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))
OBJS := $(OBJS:.c=.o)

# Set the target executable name
TARGET = $(BIN_DIR)/SmartTraffic.exe

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Ensure the build directory exists before compiling
$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up all generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: clean
