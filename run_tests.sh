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
    echo " "
    pioenv="desktop"
fi

# Initialize the counters
declare -A counts=(["Tests"]=0 ["Failures"]=0 ["Ignored"]=0 ["Projects"]=0 ["FailedProjects"]=0)

# Define the AWK command to increment the correct counter for matching lines
awkcmd='
  /([0-9]+)[[:space:]]+(Tests|Failures|Ignored)/ {
    result[$2] += $1
    result[$4] += $3
    result[$6] += $5
  } END {
    for (type in result) {
      printf "%s=%d\n", type, result[type]
    }
  }'

for folder in $(find . -name "test" -type d -print); do
    # Check if the parent directory does not contain ".pio"
    if [[ $folder != *".pio"* ]]; then
        folder=$folder/..
        echo -e "Processing tests under $folder folder."
        echo
        # Execute pio command and process the output
        while IFS="=" read -r type count
        do
            counts[$type]=$(( counts[type] + count ))
            if [[ "$type" == "Failures" && "$count" -ne 0 ]]; then
                counts["FailedProjects"]=$(( counts["FailedProjects"] + 1))
                echo "FailedProjects [$type;$count]"
            fi
        done < <(run_project_tests $pioenv $folder | tee /dev/fd/2 | awk "$awkcmd")

        counts["Projects"]=$(( counts["Projects"] + 1))
        echo " "

    fi
done

# Output the counts
echo "========================= [TOTAL] ==========================="
echo "${counts["Projects"]} Projectes ${counts["FailedProjects"]} Failed"
echo "${counts["Tests"]} Tests ${counts["Failures"]} Failures ${counts["Ignored"]} Ignored"

if [[ "${counts["FailedProjects"]}" -ne 0 ]]; then
    exit 1
fi
exit 0