"""Read the preserved reference workbook measurements without recalculating them."""
import csv
from pathlib import Path
import xml.etree.ElementTree as ET
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parents[1]
ALGORITHMS = ['insertion_sort', 'merge_sort', 'quick_sort', 'heap_sort']
NS = {'s': 'http://schemas.openxmlformats.org/spreadsheetml/2006/main'}


def read_workbook(algorithm):
    name = algorithm.split('_')[0] + 'SortComplexityAnalysis.xlsx'
    with ZipFile(ROOT / 'lab' / name) as package:
        sheet = ET.fromstring(package.read('xl/worksheets/sheet1.xml'))
        rows = []
        for row in sheet.findall('s:sheetData/s:row', NS)[1:]:
            values = [float(cell.find('s:v', NS).text) for cell in row.findall('s:c', NS)]
            n, measured, log2, log10 = values
            rows.append(dict(n=int(n), average_comparisons=measured, n_log2_n=log2, n_log10_n=log10))
    return rows


def export_data():
    result = {}
    (ROOT / 'results').mkdir(exist_ok=True)
    for algorithm in ALGORITHMS:
        rows = read_workbook(algorithm)
        if [row['n'] for row in rows] != list(range(30, 1001, 10)):
            raise ValueError('Workbook size schedule does not match the reference')
        with (ROOT / 'results' / f'{algorithm}_comparisons.csv').open('w', newline='') as file:
            writer = csv.DictWriter(file, fieldnames=list(rows[0]), lineterminator='\n')
            writer.writeheader()
            writer.writerows(rows)
        result[algorithm] = rows
    return result


if __name__ == '__main__':
    export_data()
