#!/usr/bin/python3

import os
import subprocess
import re
import sys

# Path to the clox interpreter
CLOX_PATH = os.path.join('build', 'clox')

# Root directory for tests
TEST_ROOT = 'test'
# Directories to include (relative to TEST_ROOT). Empty means include all.
INCLUDE_DIRS = ['A.chapter8']  # Example: ['expressions', 'prints']
# Directories to exclude (relative to TEST_ROOT)
EXCLUDE_DIRS = []  # Example: ['scanning']

# Pattern to extract expected output
EXPECT_PATTERN = re.compile(r'// expect: (.*)')


def find_lox_files(root, include_dirs):
    """Recursively find all .lox files under root."""
    for dirpath, _, filenames in os.walk(root):
        rel_dir = os.path.relpath(dirpath, root)
        # Skip excluded directories
        if rel_dir != '.' and any(rel_dir.startswith(ex) for ex in EXCLUDE_DIRS):
            continue
        # If include_dirs is set, only include those
        if include_dirs and rel_dir != '.' and not any(rel_dir.startswith(inc) for inc in include_dirs):
            continue
        for filename in filenames:
            if filename.endswith('.lox'):
                yield os.path.join(dirpath, filename)

def extract_expected_output(filepath):
    """Extract expected output lines from a .lox file."""
    expected = []
    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            m = EXPECT_PATTERN.search(line)
            if m:
                expected.append(m.group(1).strip())
    return expected

def run_clox(filepath):
    """Run clox interpreter on a file and return output as list of lines."""
    try:
        result = subprocess.run([CLOX_PATH, filepath], capture_output=True, text=True, timeout=5)
        output = result.stdout.strip().splitlines()
        return output
    except Exception as e:
        return [f'Error running clox: {e}']

def run_tests():
    total = 0
    passed = 0
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    RESET = '\033[0m'
    # Check for command-line arguments to override INCLUDE_DIRS
    if len(sys.argv) > 1:
        include_dirs = sys.argv[1:]
    else:
        include_dirs = INCLUDE_DIRS
    tested_files = list(find_lox_files(TEST_ROOT, include_dirs))
    print(f"\nTested files ({len(tested_files)}):")
    for f in tested_files:
        print(f"  - {f}")
    print()
    for lox_file in tested_files:
        expected = extract_expected_output(lox_file)
        actual = run_clox(lox_file)
        # Only compare lines with expectations
        if expected:
            total += 1
            if expected == actual[-len(expected):]:
                print(f'{GREEN}PASS{RESET}: {lox_file}')
                passed += 1
            else:
                print(f'{RED}FAIL{RESET}: {lox_file}')
                print(f'{RED}  Expected:{RESET}', expected)
                print(f'{RED}  Actual:  {RESET}', actual[-len(expected):])
                print(f'{YELLOW}  --- File Content ---{RESET}')
                with open(lox_file, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                for idx, line in enumerate(lines, 1):
                    print(f'{YELLOW}{idx:4}: {line.rstrip()}{RESET}')
    print(f'\nSummary: {GREEN}{passed}{RESET}/{total} tests passed.')

if __name__ == '__main__':
    run_tests()
