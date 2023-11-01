CC = g++
INCLUDE_DIR = include
CFLAGS = -Wall -Werror -I$(INCLUDE_DIR)
SRC_DIR = src
BIN_DIR = bin

# Find all .cpp and .c files in the src directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.c)

# Generate corresponding .o paths in the bin directory
OBJS := $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%,$(SRCS:.cpp=.o))
OBJS := $(OBJS:.c=.o)

REG_OBJS := $(filter-out $(BIN_DIR)/SmartTrafficTest.o,$(OBJS))
TEST_OBJS := $(filter-out $(BIN_DIR)/SmartTraffic.o,$(OBJS))

# Set the target executable name
TARGET = $(BIN_DIR)/SmartTraffic.exe
TEST_TARGET = $(BIN_DIR)/tests.exe

all: $(TARGET)

$(TARGET): $(REG_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

tests: $(TEST_TARGET)
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up all generated files
clean:
	rm -rf $(BIN_DIR)

.PHONY: clean