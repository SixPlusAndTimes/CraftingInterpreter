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
INCLUDE_DIRS = ['A.chapter8', 'A.chapter9', 'A.chapter10']  # Example: ['expressions', 'prints']
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
        if include_dirs:
            # Only allow files in root if '.' is in include_dirs
            if rel_dir == '.' and '.' not in include_dirs:
                continue
            if rel_dir != '.' and not any(rel_dir.startswith(inc) for inc in include_dirs):
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
    passed_tests = []
    failed_tests = []
    failed_details = []
    for lox_file in tested_files:
        expected = extract_expected_output(lox_file)
        actual = run_clox(lox_file)
        if expected:
            total += 1
            if expected == actual[-len(expected):]:
                passed += 1
                passed_tests.append(lox_file)
            else:
                failed_tests.append(lox_file)
                # Prepare details for failed test
                detail_lines = []
                detail_lines.append(f'{RED}FAIL{RESET}: {lox_file}')
                detail_lines.append(f'{YELLOW}  --- Details ---{RESET}')
                with open(lox_file, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                expect_indices = []
                for i, line in enumerate(lines):
                    if '// expect:' in line:
                        expect_indices.append(i)
                BOLD = '\033[1m'
                EXPECT_COLOR = '\033[94m'  # Blue
                ACTUAL_COLOR = '\033[91m'  # Red
                for idx, line in enumerate(lines, 1):
                    line_out = f'{YELLOW}{idx:4}: {line.rstrip()}'
                    if (idx-1) in expect_indices:
                        exp_idx = expect_indices.index(idx-1)
                        act_val = actual[exp_idx] if exp_idx < len(actual) else ''
                        if 'expect:' in line_out:
                            line_out = line_out.replace('expect:', f'{BOLD}{EXPECT_COLOR}expect{RESET}{YELLOW}:')
                        if act_val:
                            line_out += f'  {BOLD}{ACTUAL_COLOR}actual{RESET}{YELLOW}: {act_val}'
                    detail_lines.append(f'{line_out}{RESET}')
                failed_details.append('\n'.join(detail_lines))

    # Print passed tests
    print(f'{GREEN}Passed tests ({len(passed_tests)}):{RESET}')
    for f in passed_tests:
        print(f'  {GREEN}{f}{RESET}')
    print()
    # Print failed tests and their details
    print(f'{RED}Failed tests ({len(failed_tests)}):{RESET}')
    for detail in failed_details:
        print(detail)
    print(f'\nSummary: {GREEN}{passed}{RESET}/{total} tests passed.')

if __name__ == '__main__':
    run_tests()
