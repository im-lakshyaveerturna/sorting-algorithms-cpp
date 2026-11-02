# Reference alignment and corrections

Source: [Aayush-data-eng/Design-And-Analysis-of-Algorithm](https://github.com/Aayush-data-eng/Design-And-Analysis-of-Algorithm/tree/fb018e38df9ed2223aa6991af28c2b7294e8912f),
commit `fb018e38df9ed2223aa6991af28c2b7294e8912f`.
Alignment performed on 3 November 2026. The scope is `lab/lab1.cpp`–`lab/lab7.cpp`,
the four associated Excel workbooks, and `Sorting/*.cpp`.

## Preserved

- Original algorithm structures, names, lab numbers and built-in demo inputs.
- Vector-based comparison sorts, 30–1000 size schedule, ten instances, time-based random seed.
- Reference insertion counter, strict quicksort comparison, original console formats.
- Original 20-element counting/radix/bucket demos, `float` bucket data, and one `clock()` timing.
- All 392 saved comparison averages, both logarithmic reference columns, worksheet cells/formulas,
  and native chart layouts/styles. Three workbooks are entirely unchanged.

## Necessary deviations

| File | Correction | Reason |
|---|---|---|
| `lab/lab1.cpp`–`lab/lab4.cpp` | Widen `rand()` before adding index/instance | Prevent possible signed integer overflow while retaining the mathematical generation formula |
| `lab/lab1.cpp`–`lab/lab3.cpp` and `Sorting/*.cpp` | Return zero after success | Successful programs previously reported failure to shells/build tools |
| `lab/lab5.cpp` | Replace the initialized variable-length frequency array with a vector | Original does not compile as standard C++17 |
| `lab/lab5.cpp` | Size the output buffer from `n` | Avoid writes past a fixed 20-element buffer |
| `lab/lab5.cpp` | Handle empty/negative sizes; reject negative values and excessive ranges | Avoid invalid array access/indexing and uncontrolled frequency-table allocation; supported range is documented |
| `lab/lab6.cpp` | Size output from `n`; handle empty/negative sizes and negative values | Avoid buffer overflow and invalid input access/indexing |
| `lab/lab6.cpp` | Use `long long` for the decimal exponent | Prevent overflow after the billions digit of `INT_MAX` |
| `lab/lab6.cpp` | Reject empty `getMax` calls | Helper cannot read element zero of an empty array |
| `lab/lab7.cpp` | Use `n` variable-length bucket lists; retain the insertion-sort helper | Avoid hard-coded 20-bucket and 20-item-per-bucket limits |
| `lab/lab7.cpp` | Validate finite `[0,1)` values and sizes; widen bucket-index multiplication | Prevent invalid indexing and rounding a valid fraction up to bucket `n` |
| `Sorting/mergesort.cpp` | Use `mid + 1` to copy the right half | Original starts beyond the current range |
| `Sorting/mergesort.cpp` | Correct midpoint expression, base condition and recursive bounds | Original has syntax errors and wrong bound names |
| `Sorting/insertionSort.cpp`, `Sorting/mergesort.cpp` | Explicit loop-bound cast | Remove signed/unsigned comparison warnings |
| `lab/insertionSortComplexityAnalysis.xlsx` | Use `A2:A99` and 98 matching cached category points for all three series | Original includes the header as an extra x-axis category |

No other upstream source reformatting or algorithm substitutions are applied. The source remains
an undergraduate demonstration, including ordinary recursive quicksort and its quadratic worst case.
Its preserved insertion counter adds one after every outer iteration, even when `j >= 0` fails;
it must not be described as a precise count of all executed value comparisons.

## Saved graphs versus new executions

The workbook values are upstream historical results. No original seed, arrays or per-trial records
are available, so graph exports use the saved workbook data. Fresh program executions use the
reference's random input scheme and may differ. Timing values likewise depend on the computer.

The repository adds a Makefile, regression tests, CSV/PNG/SVG exports and documentation around
these programs. The old `src/` names are symlinks, not alternative copies. The old independent
15000-element timing experiment and incompatible data/guide are retained in Git history under
`before-reference-alignment-f3d768f`.

The native insertion workbook was repaired by retaining every untouched ZIP component and
changing only its chart's category bindings/caches. Package-level regression checks guard that
preservation. This avoids losing original Excel features during whole-workbook serialization.

## Regression evidence

- Standard C++17 builds use warnings as errors.
- Every permutation through seven elements is checked for the four comparison sorts and three examples.
- 37 exact four-sort counter fixtures were captured from the unmodified upstream functions.
- Random, duplicate-heavy, empty and boundary cases are checked independently against `std::sort`.
- Sanitizers cover fixed-buffer regressions, bucket concentration, empty input and radix `INT_MAX`.
- Ten standalone executables are run and their reference output formats checked.
- Four workbooks and four CSVs are reconciled; each chart must have three 98-point series.
