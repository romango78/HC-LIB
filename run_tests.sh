#!/bin/bash

# Initialize variables
declare testcount=0
declare failedcount=0
declare ignoredcount=0
declare projectcount=0
declare projectfailed=0

for file in $(find . -name "test" -type f -print); do
    # Get the parent directory of the current file
    declare folder="$(dirname "$file")"
    # Check if the parent directory does not contain ".pio"
    if [[ "$folder" != *".pio"* ]]; then
        echo "$folder"
        pio -h
    fi
done