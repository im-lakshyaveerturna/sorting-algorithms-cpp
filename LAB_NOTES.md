# Sorting Lab Notes

These notes explain the submitted implementations at undergraduate CSE level. The conventional algorithm choices were checked against [IIT Ropar's sorting lecture](https://cse.iitrpr.ac.in/mukesh/CSL201-1718/L18-Sorting.pdf), [NPTEL/IIT Guwahati sorting material](https://archive.nptel.ac.in/content/storage2/courses/106103069/Module_5/sort.htm), and [NPTEL/IIT Madras course topics](https://www.nptel.ac.in/courses/106106127). These sources establish the algorithms taught; they do not prescribe one universal C++ coding format. The supplied `INSERTION.CPP` determines this assignment's experiment format.

## 1. Insertion sort

**Aim:** Sort an array by inserting each element into its correct position in the already sorted prefix. Measure the average counter for sizes 30 to 1,000, step 10, with ten instances each.

**Procedure:** Save `arr[i]` as `key`. Move backwards from `j = i - 1`. While `arr[j] > key`, shift `arr[j]` to `j + 1`. Insert `key` into the resulting gap.

**Dry run:** `[5, 3, 4]` becomes `[3, 5, 4]`, then `[3, 4, 5]`. The supplied counter records two shifts.

**Analysis:** Best time O(n), average/worst O(n²), auxiliary space O(1). Stable because equal elements are not shifted past each other. In-place because the algorithm needs only a few temporary variables.

**Counter convention:** This assignment counts only successful comparisons that cause shifts. The best-case counter is 0, although actual value comparisons and loop work still occur. For distinct reverse-sorted input the counter is n(n − 1)/2. Do not confuse the shift-only count with all executed value comparisons.

## 2. Merge sort

**Aim:** Sort using divide and conquer.

**Procedure:** Find `middle`. Recursively sort the left and right halves. Compare the current values in the halves, copy the smaller value into a temporary array, copy any leftovers, and copy the merged result back.

**Dry run:** `[4, 1, 3, 2]` splits into `[4, 1]` and `[3, 2]`. These become `[1, 4]` and `[2, 3]`, then merge into `[1, 2, 3, 4]`.

**Analysis:** T(n) = 2T(n/2) + O(n), giving O(n log n) in best, average and worst cases. Textbook auxiliary storage is O(n), plus recursion. This lab reserves a fixed temporary capacity of 1,000 elements. Stable because equal values from the left half are chosen first. Not in-place in this implementation.

**Counter:** One increment each time the two current merge values are compared. Copying leftovers does not count.

## 3. Quick sort

**Aim:** Sort using partitioning and recursion.

**Procedure:** Use the last element as pivot. Scan the range, moving values less than or equal to the pivot into the left portion. Place the pivot after that portion, then recursively sort both sides. This is Lomuto partitioning, a standard textbook variant.

**Dry run:** `[4, 1, 3, 2]` with pivot 2 partitions into `[1, 2, 3, 4]`. Further recursive calls sort the portions on each side.

**Analysis:** Best/average O(n log n), worst O(n²). Balanced partitions give T(n) = 2T(n/2) + O(n); one-sided partitions give T(n) = T(n − 1) + O(n). Recursion uses O(log n) space on balanced input and O(n) in the worst case. Unstable because swaps can change the relative order of equal elements.

**Counter:** One increment per value-to-pivot comparison. Sorted and all-equal input can be worst cases with this pivot choice. Hoare partitioning and first-element pivots are also commonly taught variants, but are not used here.

## 4. Heap sort

**Aim:** Sort by repeatedly removing the maximum from a max heap.

**Procedure:** For a node at index `i`, its children are at `2*i + 1` and `2*i + 2`. Build a max heap starting from the last internal node. Swap the root with the last heap element, reduce the heap size, and restore the heap property using `heapify`.

**Dry run:** `[4, 1, 3, 2]` builds into a max heap such as `[4, 2, 3, 1]`. Repeated root extraction places 4, then 3, then 2 at the end.

**Analysis:** Bottom-up heap construction takes O(n). Repeated extraction takes O(n log n), giving O(n log n) overall. The array is rearranged in-place; this implementation's recursive `heapify` additionally uses O(log n) stack space. Unstable.

**Counter:** Count a comparison between an existing child and the current largest value. Missing-child index checks do not count.

## 5. Insertion sort timing: 15,000 elements

**Aim:** Measure execution time in milliseconds for the three cases.

**Procedure:** Use ascending values 1..15,000 for the best case, a fresh shuffled permutation for each average-case trial, and descending values for the worst case. Apply the same insertion-sort algorithm without a counter. Record five trials and print the arithmetic mean for each case.

**Timing conversion:** `chrono::duration<double, milli>(end - start).count()` gives fractional milliseconds. Generate input and verify output outside the start/end interval. A timing measurement depends on the computer and compiler; time complexity describes growth as n changes.

## 6a. Radix sort

**Aim:** Sort nonnegative integers by successive decimal digits.

**Procedure:** Find the maximum. Stable-sort by units, then tens, then hundreds, continuing until the maximum has no more digits. Each pass uses a ten-entry frequency array, prefix sums, and backward placement into the output array.

**Dry run:** `[170, 45, 75, 90]` becomes `[170, 90, 45, 75]` after units, `[45, 170, 75, 90]` after tens, then `[45, 75, 90, 170]` after hundreds.

**Analysis:** O(d(n + 10)) time and O(n + 10) auxiliary space, where d is the number of decimal digits. Stable. Stability of every digit pass is necessary to preserve ordering by the previously processed digits. This version requires nonnegative integers.

## 6b. Bucket sort

**Aim:** Sort finite fractions in [0, 1).

**Procedure:** Make n empty bucket lists. Place `arr[i]` into bucket `floor(n * arr[i])`. Insertion-sort each bucket, then concatenate the buckets in index order. A `vector<double>` represents each variable-length bucket list; the sorting within it is implemented explicitly.

**Dry run:** For `[0.42, 0.12, 0.35, 0.09]`, four buckets collect `[0.12, 0.09]`, `[0.42, 0.35]`, `[]`, `[]`. Sorting within them and concatenating gives `[0.09, 0.12, 0.35, 0.42]`.

**Analysis:** Expected O(n) for approximately uniform input with n buckets, worst O(n²) if insertion-sort work is concentrated in one bucket. Auxiliary space O(n). Stable in this implementation because distribution preserves order and the inner insertion sort is stable.

## 6c. Counting sort

**Aim:** Sort integers from a limited nonnegative range.

**Procedure:** Find the maximum, count the frequency of each value, form cumulative counts, and place input values into their output positions while scanning the input backwards. Copy the result back.

**Dry run:** `[2, 1, 2, 0]` has frequencies `[1, 1, 2]` and prefix sums `[1, 2, 4]`; placement produces `[0, 1, 2, 2]`.

**Analysis:** O(n + k) time and auxiliary space, where k = maximum + 1. Stable because placement scans backwards. Efficient when the value range is reasonably small; this lab limits values to 1,000,000.

## Measurement and graph interpretation

The first four programs report the mean of ten counters per size. The graphs compare that mean with n log₂ n and n log₁₀ n. Changing the logarithm's base only changes a constant factor; neither curve is an exact count for every implementation. Insertion's counter follows the supplied shift-only convention, while the other three count all their executed value comparisons.

Radix, bucket and counting sort each demonstrate 25 elements. Their reported milliseconds are means over 1,000 runs so small fractional times remain visible. The timer's own overhead is included; these measurements do not establish a universal speed ranking.
