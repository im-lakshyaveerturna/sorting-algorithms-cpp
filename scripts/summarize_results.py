"""Build a report from the preserved reference measurements, not a fresh run."""
from extract_reference_data import ROOT, ALGORITHMS, export_data


def main():
    data = export_data()
    lines = ['# Reference sorting lab results', '',
        'These are the saved measurements from the upstream Excel workbooks at commit',
        '`fb018e38df9ed2223aa6991af28c2b7294e8912f`, preserved without changing values.',
        'They are historical source data, not measurements of a new execution here.', '',
        'The first four labs use sizes 30 through 1000, step 10, ten random instances per size.',
        'Insertion uses the reference counter: successful shifts plus one per insertion.',
        'Quick sort uses `< pivot`. The source uses `srand(time(0))`; fresh runs vary.',
        'Raw trials and the original random seed are not present upstream, so they cannot be reconstructed.', '',
        '| n | Insertion | Merge | Quick | Heap | n log2 n | n log10 n |',
        '|---:|---:|---:|---:|---:|---:|---:|']
    for index, row in enumerate(data['insertion_sort']):
        values = [data[name][index]['average_comparisons'] for name in ALGORITHMS]
        values += [row['n_log2_n'], row['n_log10_n']]
        lines.append(f"| {row['n']} | " + ' | '.join(f'{v:,.2f}' for v in values) + ' |')
    lines += ['', '![Reference graphs](graphs/comparison_overview.png)', '',
        'The native Excel charts are in `lab/`. Their existing layouts and styles are retained.',
        'The insertion chart category range was corrected to exclude the header; its 98 measurements are unchanged.',
        'PNG/SVG exports plot the same values and reference formulas, with Excel-style series colours.',
        'They are not pixel-identical screenshots of Excel.', '',
        'Labs 5–7 retain the exact 20-element demo inputs, single `clock()` measurements,',
        'and original output format. Tiny durations can be zero and depend on the computer.',
        'The previous 25-element and 15000-element measurements are kept in Git history',
        'under tag `before-reference-alignment-f3d768f`.', '']
    (ROOT / 'REPORT.md').write_text('\n'.join(lines))
    print('Generated REPORT.md from preserved upstream workbook data')


if __name__ == '__main__':
    main()
