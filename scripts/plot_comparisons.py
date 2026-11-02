"""Plot the exact saved workbook data with the reference's three line series."""
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import StrMethodFormatter
from extract_reference_data import ALGORITHMS, ROOT, export_data

plt.rcParams['svg.hashsalt'] = 'reference-sorting-labs'


def save(figure, path):
    metadata = {'Date': None} if path.suffix == '.svg' else None
    figure.savefig(path, dpi=180, metadata=metadata)
    if path.suffix == '.svg':
        path.write_text('\n'.join(line.rstrip() for line in path.read_text().splitlines()) + '\n')


def draw(axis, algorithm, rows):
    n = [row['n'] for row in rows]
    for column, label, color in [
        ('average_comparisons', 'Number of Comparisons', '#4472C4'),
        ('n_log2_n', 'n log2 n', '#ED7D31'),
        ('n_log10_n', 'n log10 n', '#A5A5A5'),
    ]:
        axis.plot(n, [row[column] for row in rows], color=color, label=label, linewidth=1.8)
    axis.set_title(algorithm.replace('_', ' ').title() + ' Complexity Analysis')
    axis.set_xlabel('Number of Elements (n)')
    axis.set_ylabel('Comparisons / reference function value')
    axis.set_xlim(30, 1000)
    axis.set_ylim(bottom=0)
    axis.yaxis.set_major_formatter(StrMethodFormatter('{x:,.0f}'))
    axis.grid(axis='y', color='#D9D9D9', linewidth=0.7)
    axis.spines[['top', 'right']].set_visible(False)
    axis.legend(loc='upper center', bbox_to_anchor=(0.5, -0.19), ncol=3, fontsize=8, frameon=False)


def main():
    data = export_data()
    destination = ROOT / 'graphs'
    destination.mkdir(exist_ok=True)
    overview, axes = plt.subplots(2, 2, figsize=(13, 9), layout='constrained')
    for name, axis in zip(ALGORITHMS, axes.flat):
        figure, single = plt.subplots(figsize=(9, 5.5), layout='constrained')
        draw(single, name, data[name])
        for extension in ['png', 'svg']:
            save(figure, destination / f'{name}_comparisons.{extension}')
        plt.close(figure)
        draw(axis, name, data[name])
    overview.suptitle('Reference workbook measurements (saved source data)')
    for extension in ['png', 'svg']:
        save(overview, destination / f'comparison_overview.{extension}')
    plt.close(overview)
    print('Saved four graphs and overview from the original workbook measurements')


if __name__ == '__main__':
    main()
