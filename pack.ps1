# Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
#
# This software is the confidential and proprietary information of Roman Gorielov.
# It is furnished under license and may only be used or copied in accordance
# with the terms of such license.
# This software is subject to change without notice and no information
# contained in it should be construed as commitment by Roman Gorielov.

$script:pio = Join-Path $env:USERPROFILE ".platformio\penv\Scripts\pio.exe"
if (!(Test-Path $script:pio)) {
    $pioCommand = Get-Command pio -ErrorAction SilentlyContinue
    if ($pioCommand) {
        $script:pio = $pioCommand.Source
    }
    else {
        Write-Host "[ERROR]" -ForegroundColor Red -NoNewline
        Write-Host " PlatformIO CLI was not found. Install PlatformIO or add pio to PATH."
        exit 1
    }
}

$script:packages = Join-Path $PSScriptRoot "packages"
if (!(Test-Path $script:packages)) {
    New-Item -ItemType Directory -Path $script:packages | Out-Null
    Write-Host "Created $script:packages folder."
}

$script:packed = 0
$script:failed = 0

Get-ChildItem $PSScriptRoot -Directory | ForEach-Object {
    $manifest = Join-Path $_.FullName "library.json"
    if (!(Test-Path $manifest)) {
        return
    }

    Write-Host "Packing $($_.Name)..." -BackgroundColor White -ForegroundColor Black
    & $script:pio package pack $_.FullName --output $script:packages
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[FAILED]" -ForegroundColor Red -NoNewline
        Write-Host " $($_.Name)"
        $script:failed++
    }
    else {
        $script:packed++
    }
    Write-Host ""
}

Write-Host "========================= [TOTAL] ==========================="
Write-Host "$script:packed Packed $script:failed Failed"

if ($script:failed -gt 0) {
    exit 1
}
