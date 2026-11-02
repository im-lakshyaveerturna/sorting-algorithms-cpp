# Sorting lab notes

These notes follow the reference-aligned code in `lab/`.

| Lab | Method | Time | Auxiliary space |
|---|---|---|---|
| 1 | Insert each key into a sorted prefix | Best O(n), average/worst O(n²) | O(1) |
| 2 | Recursively split and merge sorted halves | O(n log n) | O(n), plus recursion |
| 3 | Lomuto partition using last pivot, strict `<` | Average O(n log n), worst O(n²) | Average O(log n), worst O(n) recursion |
| 4 | Build a max heap and repeatedly extract the largest | O(n log n) | O(log n) recursive heapify |
| 5 | Frequency counts, prefix sums, backward placement | O(n + k) | O(n + k) |
| 6 | Stable counting sort by successive decimal digits | O(d(n + 10)) | O(n + 10) |
| 7 | Distribute fractions, insertion-sort each bucket, concatenate | Expected O(n) for uniform input, worst O(n²) | O(n) |

Here `k` is the value range and `d` is the number of decimal digits.
Insertion, merge, counting, radix and this bucket implementation are stable. Quick and heap are not.

## Comparison counters

Insertion preserves the reference convention: increment for each successful shift, then increment
once after each outer iteration. Thus sorted `[1,2,3]` records 2 and reversed `[3,2,1]` records 5.
For a nonempty array it equals the inversion count plus `n - 1`. This is an instrumentation
convention, not a precise count of every executed key comparison.

Merge counts each comparison of the current values in its two halves. Quick counts each
value-to-pivot comparison, using strict `< pivot`. Heap counts each existing-child comparison
with the current largest value. Index checks, assignments and swaps are not counted.

## Experiments and graphs

Labs 1–4 run ten instances per size, from 30 to 1000 in increments of 10. Inputs use the
reference's time-based seed and random-plus-index/instance formula with a safe wider intermediate.
Labs 1–2 print size labels, while labs 3–4 print only averages, matching the reference.

The saved graphs use the reference workbook averages and logarithmic formulas. They are not
recomputed from fresh random runs. `n log2 n` and `n log10 n` differ by a constant factor,
so both represent O(n log n); neither is an exact predicted comparison count.

Labs 5–7 keep their original distinct 20-element inputs and one `clock()` timing each.
Small durations may round to zero. These demos do not establish a universal speed ranking.

See `REFERENCE_ALIGNMENT.md` for bug fixes and `REPORT.md` for the unchanged source measurements.
