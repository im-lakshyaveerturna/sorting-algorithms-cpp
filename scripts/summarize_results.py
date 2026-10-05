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
    insertion = validate_timings("insertion_sort_timing.csv", "insertion_sort_timing_trials.csv", "case")
    small = []
    for name in ["radix_sort", "bucket_sort", "counting_sort"]:
        small.extend(validate_timings(f"{name}_timing.csv", f"{name}_timing_trials.csv", "algorithm"))

    lines = [
        "# Sorting Algorithms: Lab Results", "",
        "## Experiment setup", "",
        "- Comparison inputs: shuffled permutations of 1 through n; seed 20261005.",
        f"- Sizes: {', '.join(n for n, _ in baseline)}; {baseline[0][1]} trials per size.",
        "- The exact same inputs are generated for each comparison algorithm on this build.",
        "- Count only executed comparisons between array values, including unsuccessful value comparisons.",
        "- Average comparisons = total key comparisons across trials / number of trials.",
        "- Timing: steady_clock, expressed in milliseconds, with a warmup before each case.",
        "- Input creation, copying, validation and printing are outside the timed region.",
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
        "Insertion sort grows quadratically on these random permutations. Merge, quick and heap sort show the expected n log n average growth. The two logarithmic reference curves differ only by a constant factor: n log₂ n is about 3.322 times n log₁₀ n. They are reference functions, not exact predictions of an algorithm's comparison count.",
        "", "## Insertion sort: 15,000 elements", "",
        "| Case | Input order | Trials | Mean time (ms) |", "|---|---|---:|---:|"]
    order = {"best": "Ascending", "average": "Fresh random permutation per trial", "worst": "Descending"}
    for row in insertion:
        lines.append(f"| {row['case'].title()} | {order[row['case']]} | {row['trials']} | {float(row['average_milliseconds']):.6f} |")
    lines += ["", "The best case needs n − 1 comparisons and runs in O(n). The worst case needs n(n − 1)/2 comparisons, and the average and worst cases run in O(n²). For 15,000 distinct elements, these best/worst counts are 14,999 and 112,492,500 respectively. The separate timing implementation does not maintain a comparison counter.",
        "", "## Radix, bucket and counting sort", "",
        "| Algorithm | Elements | Trials | Mean time (ms) |", "|---|---:|---:|---:|"]
    for row in small:
        lines.append(f"| {row['algorithm'].replace('_', ' ').title()} | {row['n']} | {row['trials']} | {float(row['average_milliseconds']):.6f} |")
    lines += ["", "Each program sorts 25 elements. Radix and counting sort use the same integer input. Bucket sort uses those values divided by 1000, to satisfy its [0, 1) input domain. Inputs and sorted outputs appear in the README and in program output.",
        "", "These tiny timings are machine-specific and include timer overhead. Averaging 1,000 runs reduces noise, but the results do not establish a general speed ranking.",
        "", "## Validation and limitations", "",
        "- All eight programs compile with C++17, -O2, -Wall, -Wextra and -Wpedantic.",
        "- Correctness tests cover all permutations through seven elements for the four comparison sorts, random duplicate-heavy inputs, empty and singleton arrays, and integer extremes.",
        "- Tests verify independent exact comparison counts and rejection of unsupported noncomparison inputs.",
        "- AddressSanitizer and UndefinedBehaviorSanitizer checks passed on the submitted implementation.",
        "- This report generator verifies every saved average against the raw CSV trials, and verifies both reference formulas.",
        "- The requested input sizes were not supplied, so these are documented default sizes. Re-run with assigned sizes if necessary.",
        "- C++ shuffle sequences can differ between standard-library implementations; the seed reproduces the inputs on the same implementation.",
        "- Quick sort uses a last-element pivot and can still take O(n²) time on sorted or equal inputs; smaller-partition recursion limits stack use.",
        ""]
    (ROOT / "REPORT.md").write_text("\n".join(lines), encoding="utf-8")
    print("Validated all raw measurements and generated REPORT.md")


if __name__ == "__main__":
    main()
