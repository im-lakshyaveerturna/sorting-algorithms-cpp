"""Validate saved measurements and generate the college lab report."""
import csv
import math
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ALGORITHMS = ["insertion_sort", "merge_sort", "quick_sort", "heap_sort"]


def read(filename):
    with (ROOT / "results" / filename).open(newline="") as file:
        return list(csv.DictReader(file))


def validate_timings(summary_name, samples_name, label_column):
    summaries = read(summary_name)
    groups = defaultdict(list)
    for row in read(samples_name):
        elapsed = float(row["milliseconds"])
        if not math.isfinite(elapsed) or elapsed < 0:
            raise ValueError("Invalid timing measurement")
        groups[(row[label_column], row["n"])].append(row)
    for row in summaries:
        trials = int(row["trials"])
        samples = groups.pop((row[label_column], row["n"]))
        if sorted(int(sample["trial"]) for sample in samples) != list(range(1, trials + 1)):
            raise ValueError("Missing or duplicate timing trial")
        average = sum(float(sample["milliseconds"]) for sample in samples) / trials
        if not math.isclose(average, float(row["average_milliseconds"]), abs_tol=0.000001):
            raise ValueError("Timing summary does not match its raw samples")
    if groups:
        raise ValueError("Unexpected timing trials")
    return summaries


def main():
    comparisons = {}
    for algorithm in ALGORITHMS:
        summaries = read(f"{algorithm}_comparisons.csv")
        groups = defaultdict(list)
        for row in read(f"{algorithm}_comparison_trials.csv"):
            groups[int(row["n"])].append(row)
        for row in summaries:
            n, trials = int(row["n"]), int(row["trials"])
            samples = groups.pop(n)
            if sorted(int(sample["trial"]) for sample in samples) != list(range(1, trials + 1)):
                raise ValueError("Missing or duplicate comparison trial")
            average = sum(int(sample["comparisons"]) for sample in samples) / trials
            for actual, expected in [
                (float(row["average_comparisons"]), average),
                (float(row["n_log2_n"]), n * math.log2(n)),
                (float(row["n_log10_n"]), n * math.log10(n)),
            ]:
                if not math.isclose(actual, expected, abs_tol=0.000001):
                    raise ValueError("Invalid comparison summary or reference curve")
        if groups:
            raise ValueError("Unexpected comparison trials")
        comparisons[algorithm] = summaries
    baseline = [(row["n"], row["trials"]) for row in comparisons[ALGORITHMS[0]]]
    for rows in comparisons.values():
        if [(row["n"], row["trials"]) for row in rows] != baseline:
            raise ValueError("Algorithms were run with different experiment settings")
    expected_sizes = list(range(30, 1001, 10))
    if [(int(n), int(trials)) for n, trials in baseline] != [(n, 10) for n in expected_sizes]:
        raise ValueError("Comparison schedule must match INSERTION.CPP")
    for algorithm in ALGORITHMS:
        output = (ROOT / "results" / f"{algorithm}_output.txt").read_text().splitlines()
        if len(output) != len(expected_sizes):
            raise ValueError("Incorrect two-column output length")
        for line, row in zip(output, comparisons[algorithm]):
            columns = line.split()
            if len(columns) != 2 or int(columns[0]) != int(row["n"]):
                raise ValueError("Invalid two-column output")
            if not math.isclose(float(columns[1]), float(row["average_comparisons"]), abs_tol=0.005):
                raise ValueError("Two-column output differs from CSV")
    insertion = validate_timings("insertion_sort_timing.csv", "insertion_sort_timing_trials.csv", "case")
    small = []
    for name in ["radix_sort", "bucket_sort", "counting_sort"]:
        small.extend(validate_timings(f"{name}_timing.csv", f"{name}_timing_trials.csv", "algorithm"))

    lines = [
        "# Sorting Algorithms: Lab Results", "",
        "## Experiment setup", "",
        "- Comparison inputs: rand() % 1000, allowing duplicates; srand(20261005) for reproducibility.",
        "- Sizes: 30 through 1000 inclusive, in steps of 10; ten trials per size (98 sizes).",
        "- The exact same inputs are generated for each comparison algorithm on this build.",
        "- Insertion follows the supplied INSERTION.CPP: increment only inside the shifting loop. This counts successful comparisons/shifts and excludes failed value comparisons.",
        "- Merge, quick and heap count each executed value comparison, including false outcomes. These counters therefore use a different convention from the supplied insertion example.",
        "- Average comparisons = total recorded counter across ten trials / 10.",
        "- Timing: steady_clock, expressed in milliseconds, with a warmup before each case. Each program is self-contained and uses arrays and ordinary functions.",
        "- Input creation, copying, validation and printing are outside the timed region. Random insertion-timing input uses a simple Fisher-Yates shuffle.",
        "- Memory allocation required by a sorting algorithm is inside the timed region.",
        "- Sorting is checked against the complete expected output after every measured run.",
        "", "## Average comparisons", "",
        "| n | Insertion | Merge | Quick | Heap | n log₂ n | n log₁₀ n |",
        "|---:|---:|---:|---:|---:|---:|---:|",
    ]
    for i, row in enumerate(comparisons[ALGORITHMS[0]]):
        values = [float(comparisons[name][i]["average_comparisons"]) for name in ALGORITHMS]
        values.extend([float(row["n_log2_n"]), float(row["n_log10_n"])])
        lines.append(f"| {row['n']} | " + " | ".join(f"{value:,.2f}" for value in values) + " |")
    lines += ["", "![Comparison graphs](graphs/comparison_overview.png)", "",
        "Insertion sort grows quadratically on these random arrays. Merge, quick and heap sort show the expected n log n average growth. The two logarithmic reference curves differ only by a constant factor: n log₂ n is about 3.322 times n log₁₀ n. They are reference functions, not exact predictions of an algorithm's comparison count.",
        "", "## Insertion sort: 15,000 elements", "",
        "| Case | Input order | Trials | Mean time (ms) |", "|---|---|---:|---:|"]
    order = {"best": "Ascending", "average": "Fresh random permutation per trial", "worst": "Descending"}
    for row in insertion:
        lines.append(f"| {row['case'].title()} | {order[row['case']]} | {row['trials']} | {float(row['average_milliseconds']):.6f} |")
    lines += ["", "The best case runs in O(n); the average and worst cases run in O(n²). Under the supplied shift-count convention, the best case records 0 and the reverse-sorted distinct worst case records n(n − 1)/2. The separate timing implementation does not maintain a counter.",
        "", "## Radix, bucket and counting sort", "",
        "| Algorithm | Elements | Trials | Mean time (ms) |", "|---|---:|---:|---:|"]
    for row in small:
        lines.append(f"| {row['algorithm'].replace('_', ' ').title()} | {row['n']} | {row['trials']} | {float(row['average_milliseconds']):.6f} |")
    lines += ["", "Each program sorts 25 elements. Radix and counting sort use the same integer input. Bucket sort uses those values divided by 1000, to satisfy its [0, 1) input domain, and allocates one bucket list per input element. Inputs and sorted outputs appear in the README and in program output.",
        "", "These tiny timings are machine-specific and include timer overhead. Averaging 1,000 runs reduces noise, but the results do not establish a general speed ranking.",
        "", "## Validation and limitations", "",
        "- All eight programs compile with C++17, -O2, -Wall, -Wextra and -Wpedantic.",
        "- Correctness tests cover all permutations through seven elements for the four comparison sorts, random duplicate-heavy inputs, empty and singleton arrays, and integer extremes.",
        "- Tests verify independent exact comparison counts, insertion counters against inversion counts, rejection of unsupported noncomparison inputs, and the three 15,000-element input orders.",
        "- AddressSanitizer and UndefinedBehaviorSanitizer checks passed on the submitted implementation.",
        "- This report generator verifies every saved average against the raw CSV trials, and verifies both reference formulas.",
        "- The comparison setup and insertion counter match the supplied INSERTION.CPP. A fixed-capacity array replaces its nonstandard variable-length array; a fixed random seed makes the saved results reproducible.",
        "- rand() sequences can differ between C library implementations; the seed reproduces comparison inputs on the same implementation.",
        "- Quick sort uses ordinary recursion and a last-element pivot; sorted or equal inputs can take O(n²) time and O(n) recursion depth.",
        ""]
    (ROOT / "REPORT.md").write_text("\n".join(lines), encoding="utf-8")
    print("Validated all raw measurements and generated REPORT.md")


if __name__ == "__main__":
    main()
