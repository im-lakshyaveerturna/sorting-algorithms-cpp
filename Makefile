CXX = c++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror
PYTHON ?= python3
LABS = lab1 lab2 lab3 lab4 lab5 lab6 lab7
EXAMPLES = bubbleSorst insertionSort mergesort
BINARIES = $(addprefix build/,$(LABS) $(EXAMPLES))

.PHONY: all run test sanitize graphs clean
all: $(BINARIES)

build:
	mkdir -p build

build/lab%: lab/lab%.cpp | build
	$(CXX) $(CXXFLAGS) $< -o $@

build/%: Sorting/%.cpp | build
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for program in $(LABS); do ./build/$$program || exit; done

test: all
	CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" $(PYTHON) tests/test_sorts.py

sanitize:
	$(MAKE) -B test CXXFLAGS="-std=c++17 -O1 -g -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer"

graphs:
	$(PYTHON) scripts/plot_comparisons.py

clean:
	rm -rf build
