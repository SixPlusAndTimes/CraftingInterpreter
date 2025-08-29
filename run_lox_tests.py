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
INCLUDE_DIRS = ['A.chapter8', 'A.chapter9', 'A.chapter10', 'A.chapter11']  # Example: ['expressions', 'prints']
# Directories to exclude (relative to TEST_ROOT)
EXCLUDE_DIRS = []  # Example: ['scanning']

# Patterns to extract expected output and runtime errors
EXPECT_PATTERN = re.compile(r'// expect: (.*)')
EXPECT_RUNTIME_ERROR_PATTERN = re.compile(r'// expect runtime error: (.*)')
EXPECT_PATTERN_RUNTIME_ERROR = re.compile(r'// expect: RuntimeError: (.*)')
EXPECT_ERROR_PATTERN = re.compile(r'// expect error')


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


def extract_expected(filepath):
    """Extract expected output and error lines from a .lox file."""
    expected_out = []
    expected_err = []
    runtime_error = False
    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            # Normal output expectation
            m = EXPECT_PATTERN.search(line)
            if m:
                expected_out.append(m.group(1).strip())
            # Runtime error expectation (canonical)
            m_err = EXPECT_RUNTIME_ERROR_PATTERN.search(line)
            if m_err:
                expected_err.append(m_err.group(1).strip())
                runtime_error = True
            # Runtime error expectation (legacy form)
            m_legacy = EXPECT_PATTERN_RUNTIME_ERROR.search(line)
            if m_legacy:
                expected_err.append(m_legacy.group(1).strip())
                runtime_error = True
            if EXPECT_ERROR_PATTERN.search(line):
                # Generic error expected (no specific message)
                expected_err.append('error')
                runtime_error = True
    return expected_out, expected_err, runtime_error


def run_clox(filepath):
    """Run clox interpreter on a file and return (stdout_lines, stderr_lines)."""
    try:
        result = subprocess.run([CLOX_PATH, filepath], capture_output=True, text=True, timeout=5)
        output = result.stdout.strip().splitlines()
        errput = result.stderr.strip().splitlines()
        return output, errput
    except Exception as e:
        return [f'Error running clox: {e}'], []

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
    unexamined_tests = []

    for lox_file in tested_files:
        expected_out, expected_err, runtime_error = extract_expected(lox_file)
        actual_out, actual_err = run_clox(lox_file)
        test_passed = False
        if expected_out or expected_err:
            total += 1
            if runtime_error:
                # For runtime error tests, check if expected error appears in stderr or stdout
                combined_err = actual_err + actual_out
                if expected_err:
                    # If specific error message(s) expected
                    test_passed = all(any(exp in err for err in combined_err) for exp in expected_err)
                else:
                    # If only generic error expected
                    test_passed = any('error' in err.lower() for err in combined_err)
            else:
                # For normal output tests
                test_passed = (expected_out == actual_out[-len(expected_out):])
            if test_passed:
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
                BOLD = '\033[1m'
                EXPECT_COLOR = '\033[94m'  # Blue
                ACTUAL_COLOR = '\033[91m'  # Red
                for idx, line in enumerate(lines, 1):
                    line_out = f'{YELLOW}{idx:4}: {line.rstrip()}'
                    if '// expect:' in line:
                        exp_idx = sum(1 for i in range(idx) if '// expect:' in lines[i-1]) - 1
                        act_val = actual_out[exp_idx] if 0 <= exp_idx < len(actual_out) else ''
                        if 'expect:' in line_out:
                            line_out = line_out.replace('expect:', f'{BOLD}{EXPECT_COLOR}expect{RESET}{YELLOW}:')
                        if act_val:
                            line_out += f'  {BOLD}{ACTUAL_COLOR}actual{RESET}{YELLOW}: {act_val}'
                        else:
                            line_out += f'  {BOLD}{ACTUAL_COLOR}actual{RESET}{YELLOW}: <no output>'
                    if '// expect runtime error:' in line or '// expect error' in line:
                        exp_idx = sum(1 for i in range(idx) if ('// expect runtime error:' in lines[i-1] or '// expect error' in lines[i-1])) - 1
                        act_val = actual_err[exp_idx] if 0 <= exp_idx < len(actual_err) else ''
                        if 'expect runtime error:' in line_out:
                            line_out = line_out.replace('expect runtime error:', f'{BOLD}{EXPECT_COLOR}expect runtime error{RESET}{YELLOW}:')
                        if 'expect error' in line_out:
                            line_out = line_out.replace('expect error', f'{BOLD}{EXPECT_COLOR}expect error{RESET}{YELLOW}')
                        if act_val:
                            line_out += f'  {BOLD}{ACTUAL_COLOR}actual error{RESET}{YELLOW}: {act_val}'
                        else:
                            line_out += f'  {BOLD}{ACTUAL_COLOR}actual error{RESET}{YELLOW}: <no error output>'
                    detail_lines.append(f'{line_out}{RESET}')
                failed_details.append('\n'.join(detail_lines))
        else:
            unexamined_tests.append(lox_file)

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

    # Print unexamined tests
    if unexamined_tests:
        print(f'\n{YELLOW}Unexamined tests (no expected output or error):{RESET}')
        for f in unexamined_tests:
            print(f'  {YELLOW}{f}{RESET}')

if __name__ == '__main__':
    run_tests()
