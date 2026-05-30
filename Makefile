CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude -MMD -MP
TARGET = decision_tree
SRCS = src/main.cpp src/Model.cpp src/Gini.cpp src/ReadPassenger.cpp src/ExportTree.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

.PHONY: all clean debug sanitize

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -MMD -MP emits a .d file of header dependencies per object, so editing any
# header triggers the right rebuilds automatically (no hand-listed prereqs).
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build: no optimization + debug symbols (builds on any g++/clang/MinGW).
debug: clean
	$(MAKE) CXXFLAGS="-std=c++17 -Wall -Wextra -g -O0 -Iinclude -MMD -MP"

# Sanitized build: AddressSanitizer + UBSan to catch memory/UB bugs. Needs a
# toolchain that ships the sanitizer runtimes — g++/clang on Linux/macOS;
# MinGW often lacks libasan and MSVC is unsupported.
sanitize: clean
	$(MAKE) CXXFLAGS="-std=c++17 -Wall -Wextra -g -O0 -Iinclude -MMD -MP -fsanitize=address,undefined"

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) $(TARGET).exe

-include $(DEPS)
