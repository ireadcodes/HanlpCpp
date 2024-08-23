CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

TARGETS_CH10_01 = DemoTextClustering
SRCS_CH10_01 = test/ch10/$(TARGETS_CH10_01).cpp hanlp/mining/cluster/ClusterAnalyzer.h
$(TARGETS_CH10_01): test/ch10/$(TARGETS_CH10_01).cpp
	$(CXX) $(CXXFLAGS) $(SRCS_CH10_01) -o build/$(TARGETS_CH10_01)

TARGETS = $(TARGETS_CH10_01)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean