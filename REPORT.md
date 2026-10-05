# Sorting Algorithms: Lab Results

## Experiment setup

- Comparison inputs: shuffled permutations of 1 through n; seed 20261005.
- Sizes: 100, 500, 1000, 2000, 5000, 10000; 10 trials per size.
- The exact same inputs are generated for each comparison algorithm on this build.
- Count only executed comparisons between array values, including unsuccessful value comparisons.
- Average comparisons = total key comparisons across trials / number of trials.
- Timing: steady_clock, expressed in milliseconds, with a warmup before each case.
- Input creation, copying, validation and printing are outside the timed region.
- Memory allocation required by a sorting algorithm is inside the timed region.
- Sorting is checked against the complete expected output after every measured run.

## Average comparisons

| n | Insertion | Merge | Quick | Heap | n log₂ n | n log₁₀ n |
|---:|---:|---:|---:|---:|---:|---:|
| 100 | 2,558.70 | 544.30 | 639.80 | 1,026.00 | 664.39 | 200.00 |
| 500 | 63,576.80 | 3,853.60 | 4,862.10 | 7,433.30 | 4,482.89 | 1,349.49 |
| 1000 | 251,362.90 | 8,708.40 | 10,789.50 | 16,852.30 | 9,965.78 | 3,000.00 |
| 2000 | 1,007,071.40 | 19,423.60 | 24,214.40 | 37,701.70 | 21,931.57 | 6,602.06 |
| 5000 | 6,245,262.60 | 55,223.20 | 71,321.70 | 107,663.60 | 61,438.56 | 18,494.85 |
| 10000 | 25,083,625.40 | 120,465.90 | 156,083.80 | 235,368.60 | 132,877.12 | 40,000.00 |

![Comparison graphs](graphs/comparison_overview.png)

Insertion sort grows quadratically on these random permutations. Merge, quick and heap sort show the expected n log n average growth. The two logarithmic reference curves differ only by a constant factor: n log₂ n is about 3.322 times n log₁₀ n. They are reference functions, not exact predictions of an algorithm's comparison count.

## Insertion sort: 15,000 elements

| Case | Input order | Trials | Mean time (ms) |
|---|---|---:|---:|
| Best | Ascending | 5 | 0.006475 |
| Average | Fresh random permutation per trial | 5 | 22.889134 |
| Worst | Descending | 5 | 38.572533 |

The best case needs n − 1 comparisons and runs in O(n). The worst case needs n(n − 1)/2 comparisons, and the average and worst cases run in O(n²). For 15,000 distinct elements, these best/worst counts are 14,999 and 112,492,500 respectively. The separate timing implementation does not maintain a comparison counter.

## Radix, bucket and counting sort

| Algorithm | Elements | Trials | Mean time (ms) |
|---|---:|---:|---:|
| Radix Sort | 25 | 1000 | 0.000291 |
| Bucket Sort | 25 | 1000 | 0.000574 |
| Counting Sort | 25 | 1000 | 0.000884 |

Each program sorts 25 elements. Radix and counting sort use the same integer input. Bucket sort uses those values divided by 1000, to satisfy its [0, 1) input domain. Inputs and sorted outputs appear in the README and in program output.

These tiny timings are machine-specific and include timer overhead. Averaging 1,000 runs reduces noise, but the results do not establish a general speed ranking.

## Validation and limitations

- All eight programs compile with C++17, -O2, -Wall, -Wextra and -Wpedantic.
- Correctness tests cover all permutations through seven elements for the four comparison sorts, random duplicate-heavy inputs, empty and singleton arrays, and integer extremes.
- Tests verify independent exact comparison counts and rejection of unsupported noncomparison inputs.
- AddressSanitizer and UndefinedBehaviorSanitizer checks passed on the submitted implementation.
- This report generator verifies every saved average against the raw CSV trials, and verifies both reference formulas.
- The requested input sizes were not supplied, so these are documented default sizes. Re-run with assigned sizes if necessary.
- C++ shuffle sequences can differ between standard-library implementations; the seed reproduces the inputs on the same implementation.
- Quick sort uses a last-element pivot and can still take O(n²) time on sorted or equal inputs; smaller-partition recursion limits stack use.
