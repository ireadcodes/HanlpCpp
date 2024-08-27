CC := g++
CFLAGS := -g -Wall -O0 -static-libgcc -static-libstdc++
CPPFLAGS := $(CFLAGS) -std=c++17
CPPFLAGS += -I.

TEST_DIR := test
TEST_SUBDIRS := $(shell find $(TEST_DIR) -type d)
TEST_FILES := $(foreach dir,$(TEST_SUBDIRS),$(wildcard $(dir)/*.cpp))
TARGETS := $(TEST_FILES:.cpp=)

.PHONY: all clean

all: $(TARGETS)

%: %.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)
