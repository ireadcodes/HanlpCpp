CC := clang++
CFLAGS := -g -Wall -O0
CPPFLAGS := $(CFLAGS) -std=c++17
CPPFLAGS += -I.
LDFLAGS :=

TEST_DIR := test
TEST_SUBDIRS := $(shell find $(TEST_DIR) -type d)
TEST_FILES := $(foreach dir,$(TEST_SUBDIRS),$(wildcard $(dir)/*.cpp))
OBJS = $(TEST_FILES:.cpp=.o)
TARGETS := $(TEST_FILES:.cpp=.out)

.PHONY: all clean

all: $(TARGETS)

%.o: %.cpp
	$(CC) -c $< $(CPPFLAGS) -o $@

%.out: %.o
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS) $(OBJS)
