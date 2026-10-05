CXX = c++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic
PROGRAMS = insertion_sort merge_sort quick_sort heap_sort insertion_sort_timing radix_sort bucket_sort counting_sort
BINARIES = $(addprefix build/,$(PROGRAMS))

.PHONY: all run test sanitize graphs clean
all: $(BINARIES)

build:
	mkdir -p build

build/%: src/%.cpp src/experiment.h | build
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	./build/insertion_sort
	./build/merge_sort
	./build/quick_sort
	./build/heap_sort
	./build/insertion_sort_timing
	./build/radix_sort
	./build/bucket_sort
	./build/counting_sort

build/test_sorts: tests/test_sorts.cpp $(wildcard src/*.cpp) src/experiment.h | build
	$(CXX) $(CXXFLAGS) $< -o $@

test: build/test_sorts
	./build/test_sorts

sanitize: | build
	$(CXX) -std=c++17 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer tests/test_sorts.cpp -o build/test_sorts_sanitized
	./build/test_sorts_sanitized

graphs:
	python3 scripts/plot_comparisons.py

clean:
	rm -rf build
