# Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
#
# This software is the confidential and proprietary information of Roman Gorielov.
# It is furnished under license and may only be used or copied in accordance
# with the terms of such license.
# This software is subject to change without notice and no information
# contained in it should be construed as commitment by Roman Gorielov.

# Get arguments from command line
$script:pioenv = $args[0];

if(!$pioenv) {
    Write-Host "[WARNING]" -ForegroundColor Yellow -NoNewline
    Write-Host " The environment is not specified. The default 'desktop' environment will be used."
    Write-Host ""
    $pioenv = "desktop";
}

$script:testcount = 0;
$script:failedcount = 0;
$script:ignoredcount = 0;
$script:projectcount = 0;
$script:projectfailed = 0;
# Process tests
Get-ChildItem $PSScriptRoot -Filter "test" -Recurse | ForEach-Object { 
    $folder = Convert-Path $_.PSParentPath;
    if(!($folder -Match ".pio"))
    {
        $piocmd = $env:USERPROFILE + "\.platformio\penv\Scripts\pio.exe test -e " + $script:pioenv + " -d " + $folder;
        
        Write-Host "Processing tests under $folder folder." -BackgroundColor White -ForegroundColor Black
        Write-Host ""

        Invoke-Expression $piocmd | Tee-Object -Variable pioout
        $pioout | Select-String -Pattern '((\d+) (Tests|Failures|Ignored))' -AllMatches | ForEach-Object {
            if($_.Line -ne 0) {
                $script:testcount += [int]$_.Matches[0].Groups[2].Value;
                $script:failedcount += [int]$_.Matches[1].Groups[2].Value;
                $script:ignoredcount += [int]$_.Matches[2].Groups[2].Value;
            }
        }
        if($pioout | Select-String -Pattern '\sFAILED\s' -Quiet) {
            $script:projectfailed++;
        }
        $script:projectcount++;
        Write-Host ""
    }
}

Write-Host "========================= [TOTAL] ==========================="
Write-Host "$script:projectcount Projectes $script:projectfailed Failed"
Write-Host "$script:testcount Tests $script:failedcount Failures $script:ignoredcount Ignored"