# Copyright (c) 2024 Roman Gorielov. All Rights Reserved.
#
# This software is the confidential and proprietary information of Roman Gorielov.
# It is furnished under license and may only be used or copied in accordance
# with the terms of such license.
# This software is subject to change without notice and no information
# contained in it should be construed as commitment by Roman Gorielov.
#!/bin/bash

function run_project_tests()
{
    local environment=$1
    local project_dir=$2

    pio test -e $environment -d $project_dir -v
}

# Get arguments from command line
pioenv=$1

if [ -z "$pioenv" ]; then
    echo -e "[WARNING] The environment is not specified. The default 'desktop' environment will be used."
    echo '\n'
    pioenv="desktop"
fi

# Initialize variables
declare testcount=0
declare failedcount=0
declare ignoredcount=0
declare projectcount=0
declare projectfailed=0

# Initialize the counters
declare -A counts=(["Tests"]=0 ["Failures"]=0 ["Ignored"]=0)

counts["Tests"]=20
counts["Tests"]=+10

# Define the AWK command to increment the correct counter for matching lines
awkcmd='
  /([0-9]+)[[:space:]]+(Tests|Failures|Ignored)/ {
    count=$1
    type=$(NF-1)

    counts[type] += count
  }
  END {
    for (type in counts) {
      printf "%s=%d\n", type, counts[type]
    }
  }'

for folder in $(find . -name "test" -type d -print); do
    # Get the parent directory of the current file
    #echo $folder
    # Check if the parent directory does not contain ".pio"
    if [[ $folder != *".pio"* ]]; then
        folder=$folder/..
        echo -e "Processing tests under $folder folder."
        echo
        # Execute pio command and process the output
        run_project_tests $pioenv $folder | tee /dev/fd/2 | awk "$awkcmd" | while IFS="=" read -r type count; do
            echo $type
            echo $count
        #    counts[$type]=$count
        done



        echo " "

    fi
    break
done

# Output the counts
echo "${counts["Tests"]}"
echo "${counts["Failures"]}"
echo "${counts["Ignored"]}"