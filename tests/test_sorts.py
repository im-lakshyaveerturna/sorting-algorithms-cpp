"""Regression tests for the reference-aligned sorting labs and saved graphs."""
import csv
import hashlib
import json
import math
import os
import re
from pathlib import Path
import shlex
import subprocess
import xml.etree.ElementTree as ET
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build'
NS = {'s': 'http://schemas.openxmlformats.org/spreadsheetml/2006/main',
      'c': 'http://schemas.openxmlformats.org/drawingml/2006/chart'}


def check_workbooks():
    manifest = json.loads((ROOT / 'tests/fixtures/workbook_manifest.json').read_text())
    for name in ['insertion', 'merge', 'quick', 'heap']:
        file = ROOT / 'lab' / f'{name}SortComplexityAnalysis.xlsx'
        with ZipFile(file) as package:
            original = manifest[file.name]
            assert set(package.namelist()) == set(original)
            for part in package.namelist():
                if name == 'insertion' and part == 'xl/charts/chart1.xml':
                    continue
                assert hashlib.sha256(package.read(part)).hexdigest() == original[part], (name, part)
            sheet = ET.fromstring(package.read('xl/worksheets/sheet1.xml'))
            rows = sheet.findall('s:sheetData/s:row', NS)[1:]
            saved_rows = list(csv.DictReader((ROOT / 'results' / f'{name}_sort_comparisons.csv').open()))
            assert len(rows) == len(saved_rows) == 98
            for row, saved in zip(rows, saved_rows):
                n, measured, log2, log10 = [float(c.find('s:v', NS).text) for c in row.findall('s:c', NS)]
                assert float(saved['n']) == n and float(saved['average_comparisons']) == measured
                assert math.isclose(log2, n * math.log2(n), rel_tol=1e-12)
                assert math.isclose(log10, n * math.log10(n), rel_tol=1e-12)
                assert math.isclose(float(saved['n_log2_n']), log2, rel_tol=1e-12)
                assert math.isclose(float(saved['n_log10_n']), log10, rel_tol=1e-12)
            chart = ET.fromstring(package.read('xl/charts/chart1.xml'))
            series = chart.findall('.//c:ser', NS)
            assert len(series) == 3
            for s in series:
                assert s.find('c:cat//c:f', NS).text == 'Sheet1!$A$2:$A$99'
                categories = s.findall('c:cat//c:pt', NS)
                values = s.findall('c:val//c:pt', NS)
                assert len(categories) == len(values) == 98
                assert [int(float(p.find('c:v', NS).text)) for p in categories] == list(range(30, 1001, 10))
            if name == 'insertion':
                structural_xml = re.sub(rb'<c:cat>.*?</c:cat>', b'<c:cat/>', package.read('xl/charts/chart1.xml'), flags=re.S)
                expected = (ROOT / 'tests/fixtures/insertion_chart_structure.sha256').read_text().strip()
                assert hashlib.sha256(structural_xml).hexdigest() == expected
    print('PASS: original workbook parts, measurements, formulas and aligned 98-point charts')


def main():
    BUILD.mkdir(exist_ok=True)
    code = '''#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <random>
#include <numeric>
#include <limits>
#include <climits>
#include <array>
'''
    for namespace, source in [(f'lab{i}', ROOT / 'lab' / f'lab{i}.cpp') for i in range(1, 8)] + [(name, ROOT / 'Sorting' / f'{name}.cpp') for name in ['bubbleSorst', 'insertionSort', 'mergesort']]:
        code += f'namespace test_{namespace} {{\n#define main demo_main\n#include "{source}"\n#undef main\n}}\n'
    code += r'''
void require(bool ok) { if(!ok) throw std::runtime_error("Regression failed"); }
std::array<int,4> counters(std::vector<int>a) {
 auto b=a,c=a,d=a,expected=a;std::sort(expected.begin(),expected.end());
 int n=static_cast<int>(a.size()),m=0,q=0,h=0,i=lab1::insertionSort(a);
 lab2::mergeSort(b,0,n-1,m);lab3::quickSort(c,0,n-1,q);lab4::heapSort(d,n,h);
 require(a==expected&&b==expected&&c==expected&&d==expected);return {i,m,q,h};
}
void check(std::vector<int>a) {
 auto counts=counters(a);int inversions=0;
 for(std::size_t i=0;i<a.size();++i)for(std::size_t j=i+1;j<a.size();++j)if(a[i]>a[j])++inversions;
 require(counts[0]==inversions+std::max(0,static_cast<int>(a.size())-1));
 auto b=a,c=a,d=a,expected=a;std::sort(expected.begin(),expected.end());
 bubbleSorst::bubbleSort(b);insertionSort::insertionSort(c);mergesort::mergeSort(d,0,static_cast<int>(d.size())-1);
 require(b==expected&&c==expected&&d==expected);
}
int main() {
 for(int n=0;n<=7;++n){std::vector<int>a(n);std::iota(a.begin(),a.end(),0);do{check(a);}while(std::next_permutation(a.begin(),a.end()));}
 check({INT_MIN,INT_MAX,0,-1,0});check({5,5,5,5});
 std::mt19937 rng(20261103);
 for(int t=0;t<100;++t){std::vector<int>a(200);for(int&v:a)v=static_cast<int>(rng()%101)-50;check(a);}
 FIXTURES
 for(int n:{0,1,20,25,100}){
  std::vector<int>a(n);for(int&v:a)v=static_cast<int>(rng()%1001);auto b=a,expected=a;std::sort(expected.begin(),expected.end());
  lab5::countsort(a.data(),n);lab6::radixsort(b.data(),n);require(a==expected&&b==expected);
 }
 std::vector<int>a={INT_MAX,0,INT_MAX-1,9};lab6::radixsort(a.data(),4);require(std::is_sorted(a.begin(),a.end()));
 for(int n:{0,1,20,25,100,1001}){
  std::vector<float>b(n);for(float&v:b)v=static_cast<float>(rng()%1000)/1000;
  auto expected=b;std::sort(expected.begin(),expected.end());lab7::bucket_sort(b.data(),n);require(b==expected);
  std::fill(b.begin(),b.end(),0.5f);lab7::bucket_sort(b.data(),n);
 }
 float edge[]={0.0f,std::nextafter(1.0f,0.0f),0.1f};lab7::bucket_sort(edge,3);require(std::is_sorted(edge,edge+3));
 for(int value:{-1,1000001}){bool rejected=false;try{lab5::countsort(&value,1);}catch(const std::invalid_argument&){rejected=true;}require(rejected);}
 int negative=-1;bool rejected=false;try{lab6::radixsort(&negative,1);}catch(const std::invalid_argument&){rejected=true;}require(rejected);
 for(float value:{-0.1f,1.0f,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()}){
  rejected=false;try{lab7::bucket_sort(&value,1);}catch(const std::invalid_argument&){rejected=true;}require(rejected);
 }
 for(int kind=5;kind<=7;++kind){
  rejected=false;try{if(kind==5)lab5::countsort(nullptr,-1);if(kind==6)lab6::radixsort(nullptr,-1);if(kind==7)lab7::bucket_sort(nullptr,-1);}
  catch(const std::invalid_argument&){rejected=true;}require(rejected);
 }
 std::cout<<"PASS: all sorts, examples, exhaustive permutations, reference counters and boundary inputs\n";
}
'''
    statements = []
    for fixture in json.loads((ROOT / 'tests/fixtures/reference_counts.json').read_text()):
        values = ','.join(map(str, fixture['input']))
        counts = ','.join(str(fixture['counts'][kind]) for kind in ['insertion', 'merge', 'quick', 'heap'])
        statements.append(f'require(counters({{{values}}})==std::array<int,4>{{{counts}}});')
    source = BUILD / 'test_sorts.cpp'
    code = code.replace('FIXTURES', '\n'.join(statements))
    for name in ['lab1', 'lab2', 'lab3', 'lab4', 'lab5', 'lab6', 'lab7', 'bubbleSorst', 'insertionSort', 'mergesort']:
        code = code.replace(name + '::', 'test_' + name + '::')
    source.write_text(code)
    compiler = shlex.split(os.environ.get('CXX', 'c++'))
    flags = shlex.split(os.environ.get('CXXFLAGS', '-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror'))
    subprocess.run(compiler + flags + [str(source), '-o', str(BUILD / 'test_sorts')], check=True)
    subprocess.run([str(BUILD / 'test_sorts')], check=True, timeout=60)
    demo_inputs = json.loads((ROOT / 'tests/fixtures/reference_demo_inputs.json').read_text())
    for number in range(1, 8):
        result = subprocess.run([str(BUILD / f'lab{number}')], capture_output=True, text=True, check=True, timeout=30)
        if number <= 4:
            assert len(result.stdout.splitlines()) == 98
            for n, line in zip(range(30, 1001, 10), result.stdout.splitlines()):
                assert float(line.split(': ')[-1] if number <= 2 else line) >= 0
                if number <= 2: assert line.startswith(f'Length: {n}, Average Comparisons: ')
        else:
            expected = demo_inputs[f'lab{number}']
            original = result.stdout.split('Original Array:\n')[1].split('Sorted Array:')[0]
            assert list(map(float, original.split())) == expected
            text = result.stdout.split('Sorted Array:\n')[1].split('Execution Time:')[0]
            values = list(map(float, text.split()))
            assert len(values) == 20 and values == sorted(expected)
    for name, expected in [('bubbleSorst', [1,3,5,6]), ('mergesort', [10,27,38,43])]:
        result = subprocess.run([str(BUILD / name)], capture_output=True, text=True, check=True)
        assert list(map(int, result.stdout.replace('Sorted array is : ', '').split())) == expected
    result = subprocess.run([str(BUILD / 'insertionSort')], capture_output=True, text=True, check=True)
    assert '5 6 11 12 13' in result.stdout and 'Number of comparisons are : 11' in result.stdout
    print('PASS: ten standalone demos, reference output formats and 20-element inputs')
    check_workbooks()


if __name__ == '__main__':
    main()
