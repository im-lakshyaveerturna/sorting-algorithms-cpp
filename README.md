# Sorting Algorithms in C++

Eight separate, commented C++17 programs for an intermediate college Design and Analysis of Algorithms lab. The repository includes actual measurements, raw CSV trials, PNG/SVG graphs, and a [lab report](REPORT.md).

## Programs

| Task | Source file | Output |
|---|---|---|
| 1. Insertion sort | [src/insertion_sort.cpp](src/insertion_sort.cpp) | Average comparisons and graph data |
| 2. Merge sort | [src/merge_sort.cpp](src/merge_sort.cpp) | Average comparisons and graph data |
| 3. Quick sort | [src/quick_sort.cpp](src/quick_sort.cpp) | Average comparisons and graph data |
| 4. Heap sort | [src/heap_sort.cpp](src/heap_sort.cpp) | Average comparisons and graph data |
| 5. Separate insertion sort timing | [src/insertion_sort_timing.cpp](src/insertion_sort_timing.cpp) | Best, average and worst case, 15,000 elements |
| 6a. Radix sort | [src/radix_sort.cpp](src/radix_sort.cpp) | 25 elements and execution time in ms |
| 6b. Bucket sort | [src/bucket_sort.cpp](src/bucket_sort.cpp) | 25 elements and execution time in ms |
| 6c. Counting sort | [src/counting_sort.cpp](src/counting_sort.cpp) | 25 elements and execution time in ms |

Each `.cpp` has its own `main` and compiles separately. The first four programs are self-contained, using ordinary arrays and simple loops, following the supplied `INSERTION.CPP`. The remaining timing programs use [experiment.h](src/experiment.h) for timing, validation and CSV output. The sorting implementations are in the individual files. `std::sort` is used only as a correctness reference outside the timed sections.

## Compile and run

Requires a C++17 compiler (GCC, Clang or recent MSVC). Run commands from the repository root so output files go to `results/`.

```sh
make all
make test
make run
```

Or compile a single comparison program; it needs no project header:

```sh
c++ -std=c++17 -O2 -Wall -Wextra -Wpedantic src/merge_sort.cpp -o merge_sort
./merge_sort
```

Compile each of the other `.cpp` files the same way. Do not link all eight program files together, because each has a separate `main`.

The first four programs follow the supplied example:

- Array sizes from **30 through 1,000**, increasing by **10**.
- **10 random instances** per size, each filled with `rand() % 1000` (duplicates are allowed).
- A plain integer counter and `totalComparisons / 10.0` for the mean.
- Console output of **two columns**: `size averageComparisons`.
- Each program saves the same two-column output in `results/<algorithm>_output.txt`, plus summary and raw-trial CSVs for graphing.

The only input-generation change is a fixed `srand(20261005)` seed instead of `srand(time(0))`. This gives all four programs the same arrays and makes the submitted data reproducible on the same C library. To use a new sequence on every run, add `<ctime>` and change the seed call to `srand(time(0))`.

The supplied `int arr[size]` is a variable-length array, which is not standard C++. These programs use `int arr[1000]` and process only the first `size` entries. Merge sort's temporary array also has capacity 1,000; these lab implementations support at most 1,000 elements.

Run the programs without command-line arguments. To change the experiment, edit the size loop and instance loop in each file; if changing the maximum beyond 1,000, increase the array capacities too. Update the mean divisor and graph/report expectations if changing the number of trials. Running the programs overwrites their result files; regenerate the graphs and report afterward.

## Graphs and report

Python is only needed for graphs and report generation; all sorting and measurements are C++.

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python scripts/summarize_results.py
.venv/bin/python scripts/plot_comparisons.py
```

On Windows, use `.venv\Scripts\python.exe` instead. Generated graphs are already included, so Python is not required to view the submitted results.

![All four comparison graphs](graphs/comparison_overview.png)

Each graph plots average comparisons, n log₂ n and n log₁₀ n against the number of elements. Each algorithm also has its own PNG and scalable SVG in `graphs/`. Each panel uses its own vertical scale; insertion sort's much larger counts would compress the other algorithms on one shared scale.

## Comparison-count convention

Insertion sort matches the supplied `INSERTION.CPP`: `comparisons++` is inside `while (j >= 0 && arr[j] > key)`. It therefore counts **successful comparisons that cause shifts**, not all executed value comparisons. The final failed comparison is excluded. Its counter equals the number of inversions in the original array.

For example, `[1, 2, 3]` records **0**, `[3, 2, 1]` records **3**, and `[5, 3, 4]` records **2**. This convention is preserved to match the expected lab code. The best-case time is still O(n), even though this particular counter is zero.

Merge sort counts each comparison between the two current merge values; quick sort counts every value-to-pivot comparison; heap sort counts each existing-child comparison with the current maximum. Both successful and unsuccessful comparisons count for these three sorts. Loop/index checks, assignments and swaps do not count. Because insertion follows its supplied shift-only convention, the four reported counters are not identical definitions of total key comparisons.

## Timing experiments

Insertion sort uses exactly 15,000 distinct integers: ascending for the best case, a new shuffled permutation per trial for the average case, and descending for the worst case. Each mean uses five trials. The separate timing code has no comparison counter.

Radix and counting sort use these 25 integers:

```text
170 45 75 90 802 24 2 66 0 999 501 18 39 120 300 5 75 42 610 7 88 456 321 11 200
```

Their sorted result is:

```text
0 2 5 7 11 18 24 39 42 45 66 75 75 88 90 120 170 200 300 321 456 501 610 802 999
```

Bucket sort uses the same values divided by 1000. Its sorted result is:

```text
0 0.002 0.005 0.007 0.011 0.018 0.024 0.039 0.042 0.045 0.066 0.075 0.075 0.088 0.090 0.120 0.170 0.200 0.300 0.321 0.456 0.501 0.610 0.802 0.999
```

Each of these three programs measures 1,000 individual sorts and prints the mean milliseconds with six decimal places. These small durations include timer overhead and should not be interpreted as a universal performance ranking. Every timed output is checked; input copying, data generation, printing and validation are excluded. Algorithm-internal allocation is included.

## Algorithm notes

| Algorithm | Main idea | Time complexity | Auxiliary space |
|---|---|---|---|
| Insertion | Insert the next key into a sorted prefix | Best O(n); average/worst O(n²) | O(1) |
| Merge | Recursively split, then merge sorted halves | O(n log n) | O(n), plus recursion |
| Quick | Partition around the last value | Average O(n log n); worst O(n²) | Average O(log n), worst O(n) recursive stack |
| Heap | Build a max heap and repeatedly extract its maximum | O(n log n) | O(log n) recursive heapify stack |
| Radix | Stable counting sort on successive decimal digits | O(d(n + 10)) | O(n + 10) |
| Bucket | Distribute into ten buckets, insertion sort each, concatenate | O(n + b + Σ nᵢ²), b = 10 here; worst O(n²) | O(n + b) |
| Counting | Frequencies, prefix sums, stable placement | O(n + k) | O(n + k) |

`d` is the number of decimal digits, `k` the integer range size, and `nᵢ` the size of bucket i. Bucket sort's familiar expected linear complexity assumes a roughly uniform distribution and a bucket count proportional to n; this small demonstration uses ten buckets.

Radix sort accepts nonnegative `int` values, including `INT_MAX`. Counting sort accepts integers from 0 through 1,000,000 to keep memory use bounded. Bucket sort accepts finite numbers in [0, 1). The comparison sorts accept negative integers and duplicates.

## Verification

```sh
make test
make sanitize
```

Tests compare all algorithms against `std::sort`, exhaustively check permutations through seven elements for the comparison sorts, check random/duplicate/boundary cases, verify exact comparison counts, and check invalid input rejection. Sanitizer testing checks memory access and undefined behavior. The report script verifies saved means against raw trials and checks both logarithmic formulas.

## Recorded environment and reference

Measurements were collected on October 5, 2026, on macOS 26.2, ARM64, using Apple Clang 17.0.0 with `-std=c++17 -O2`. Timings will change with hardware, compiler and machine load. The random sequence may differ across C library implementations.

The user-supplied `INSERTION.CPP` defines the expected array sizes, random inputs, ten-instance experiment, output format and insertion counter. The first four comparison programs follow that structure. The initially supplied [Sorting repository](https://github.com/Aayush-data-eng/Design-And-Analysis-of-Algorithm/tree/main/Sorting) was also inspected; its merge example has compile/indexing errors.
