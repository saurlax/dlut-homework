CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

BINDIR = bin
TARGET = $(BINDIR)/disk

all: $(TARGET)
$(TARGET): main.cpp disk.cpp disk.h | $(BINDIR)
	$(CXX) $(CXXFLAGS) main.cpp disk.cpp -o $@
$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)
