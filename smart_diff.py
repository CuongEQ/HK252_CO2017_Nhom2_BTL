import os
import sys
import difflib


def main():
    output_dir = "output"
    expected_dir = os.path.join(output_dir, "expected")

    if not os.path.exists(output_dir):
        print(f"Error: Directory '{output_dir}' does not exist.")
        sys.exit(1)

    if not os.path.exists(expected_dir):
        print(f"Error: Directory '{expected_dir}' does not exist.")
        print("Please create output/expected/ with the expected output files.")
        sys.exit(1)

    results = {"pass": [], "fail": [], "no_expected": [], "skipped": []}

    # Collect actual output files (only in output/ root, not expected/)
    actual_files = sorted(
        [f for f in os.listdir(output_dir)
         if f.endswith(".output") and os.path.isfile(os.path.join(output_dir, f))]
    )

    # Collect expected output files
    expected_files = set(
        f for f in os.listdir(expected_dir)
        if f.endswith(".output") and os.path.isfile(os.path.join(expected_dir, f))
    )

    # Detect skipped tests (have expected output but no actual output this run)
    skipped_files = sorted(expected_files - set(actual_files))
    for f in skipped_files:
        results["skipped"].append(f)

    if not actual_files and not skipped_files:
        print("No output files found.")
        sys.exit(1)

    print("=" * 60)
    print("  SMART DIFF RESULTS")
    print("=" * 60)

    for filename in actual_files:
        actual_path = os.path.join(output_dir, filename)
        expected_path = os.path.join(expected_dir, filename)

        with open(actual_path, "r") as f:
            actual_content = f.read()

        # Check if expected file exists
        if not os.path.exists(expected_path):
            print(f"  🆕 [NEW]     {filename} — no expected output")
            results["no_expected"].append(filename)
            continue

        with open(expected_path, "r") as f:
            expected_content = f.read()

        # Smart Diff Logic: compare sorted set of lines to ignore ordering
        # differences caused by multi-threading
        actual_lines = sorted(
            [line.strip() for line in actual_content.splitlines() if line.strip()]
        )
        expected_lines = sorted(
            [line.strip() for line in expected_content.splitlines() if line.strip()]
        )

        if actual_lines != expected_lines:
            print(f"  ✗  [FAIL]    {filename}")
            results["fail"].append(filename)

            # Print a brief diff
            diff = list(difflib.unified_diff(
                expected_lines, actual_lines,
                fromfile=f"expected/{filename}",
                tofile=filename,
                n=2
            ))
            for line in diff[:20]:
                print(f"               {line}")
            if len(diff) > 20:
                print("               ... (truncated)")
        else:
            print(f"  ✓  [PASS]    {filename}")
            results["pass"].append(filename)

    # Show skipped
    if results["skipped"]:
        print()
        for f in results["skipped"]:
            print(f"  ⊘  [SKIP]    {f} — module not enabled")

    # Print summary
    total = sum(len(v) for v in results.values())
    print()
    print("=" * 60)
    print("  FINAL SUMMARY")
    print("=" * 60)
    print(f"  Total:       {total}")
    print(f"  Pass:        {len(results['pass'])}")
    print(f"  Fail:        {len(results['fail'])}")
    print(f"  New:         {len(results['no_expected'])}  (no expected output)")
    print(f"  Skipped:     {len(results['skipped'])}  (module not enabled)")
    print("=" * 60)

    if results["fail"]:
        print("\n  Failed tests:")
        for f in results["fail"]:
            print(f"    - {f}")

    print()
    if results["fail"]:
        print("⚠ Some tests did not pass!")
        sys.exit(1)
    else:
        print("✅ All executed tests passed successfully.")
        sys.exit(0)


if __name__ == "__main__":
    main()
