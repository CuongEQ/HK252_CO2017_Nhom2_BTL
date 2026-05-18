import os
import subprocess
import sys

def get_git_file_content(filepath):
    try:
        result = subprocess.run(['git', 'show', f'HEAD:{filepath}'], capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError:
        return None

def main():
    output_dir = 'output'
    if not os.path.exists(output_dir):
        print(f"Error: Directory {output_dir} does not exist.")
        sys.exit(1)

    all_passed = True

    for filename in os.listdir(output_dir):
        if not filename.endswith('.output'):
            continue
        
        filepath = os.path.join(output_dir, filename)
        
        with open(filepath, 'r') as f:
            actual_content = f.read()
            
        expected_content = get_git_file_content(filepath)
        
        if expected_content is None:
            print(f"[NEW] {filename} is not tracked in git yet. Run git add to track it.")
            continue
            
        # Smart Diff Logic: compare set of lines to ignore ordering differences caused by multi-threading
        actual_lines = sorted([line.strip() for line in actual_content.splitlines() if line.strip()])
        expected_lines = sorted([line.strip() for line in expected_content.splitlines() if line.strip()])
        
        if actual_lines != expected_lines:
            print(f"[FAIL] {filename} diff failed.")
            all_passed = False
            
            # Print a brief diff of the first few differences
            import difflib
            diff = list(difflib.unified_diff(expected_lines, actual_lines, n=2))
            print('\n'.join(diff[:20]))
            if len(diff) > 20:
                print("... (truncated)")
        else:
            print(f"[PASS] {filename}")
            
    if not all_passed:
        print("\nSome tests failed!")
        sys.exit(1)
    else:
        print("\nAll tests passed successfully.")
        sys.exit(0)

if __name__ == "__main__":
    main()
