CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = decision_tree
SRCS = main.cpp Model.cpp Gini.cpp ReadPassenger.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp DecisionTree.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
