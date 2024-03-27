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

    pio.exe test -e $environment -d $project_dir
}

# Get arguments from command line
pioenv=$1

if [ -z "$pioenv" ]; then
    echo -e "[WARNING] The environment is not specified. The default 'desktop' environment will be used."
    echo 
    pioenv="desktop"
fi

# Initialize variables
declare testcount=0
declare failedcount=0
declare ignoredcount=0
declare projectcount=0
declare projectfailed=0

for folder in $(find . -name "test" -type d -print); do
    # Get the parent directory of the current file
    #echo $folder
    # Check if the parent directory does not contain ".pio"
    if [[ $folder != *".pio"* ]]; then
        echo -e "Processing tests under $folder folder."
        echo 
        run_project_tests $pioenv $folder

    fi
done