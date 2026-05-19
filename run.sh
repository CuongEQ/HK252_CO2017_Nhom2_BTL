#!/bin/bash
mkdir -p output
for file in input/*; do
  if [ -f "$file" ]; then
    base=$(basename "$file")
    echo "Running test: $base"
    ./os "$base" > "output/${base}.output"
  fi
done
