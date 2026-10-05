"""Generate one graph per algorithm, plus a four-panel overview, from C++ CSVs."""
import csv
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
ALGORITHMS = ["insertion_sort", "merge_sort", "quick_sort", "heap_sort"]


def draw(axis, name, rows):
    n = [int(row["n"]) for row in rows]
    for column, label, color, marker in [
        ("average_comparisons", "Average comparisons", "#1565c0", "o"),
        ("n_log2_n", r"$n\log_2 n$", "#d84315", "s"),
        ("n_log10_n", r"$n\log_{10} n$", "#2e7d32", "^"),
    ]:
        axis.plot(n, [float(row[column]) for row in rows], label=label,
                  color=color, marker=marker, linewidth=1.8, markersize=4)
    axis.set_title(name.replace("_", " ").title())
    axis.set_xlabel("Number of elements (n)")
    axis.set_ylabel("Comparisons / reference function value")
    axis.grid(alpha=0.25)
    axis.legend(fontsize=9)
    axis.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))


def main():
    destination = ROOT / "graphs"
    destination.mkdir(exist_ok=True)
    overview, axes = plt.subplots(2, 2, figsize=(12, 8), constrained_layout=True)
    for name, axis in zip(ALGORITHMS, axes.flat):
        with (ROOT / "results" / f"{name}_comparisons.csv").open(newline="") as file:
            rows = list(csv.DictReader(file))
        if not rows:
            raise ValueError(f"No results for {name}")
        figure, single_axis = plt.subplots(figsize=(8, 5), constrained_layout=True)
        draw(single_axis, name, rows)
        figure.suptitle(f"Mean over {rows[0]['trials']} shuffled-input trials per size", fontsize=11)
        figure.savefig(destination / f"{name}_comparisons.png", dpi=180)
        figure.savefig(destination / f"{name}_comparisons.svg")
        plt.close(figure)
        draw(axis, name, rows)
    overview.suptitle("Sorting comparisons against n log₂ n and n log₁₀ n")
    overview.savefig(destination / "comparison_overview.png", dpi=180)
    overview.savefig(destination / "comparison_overview.svg")
    plt.close(overview)
    print(f"Saved PNG and SVG graphs to {destination}")


if __name__ == "__main__":
    main()
