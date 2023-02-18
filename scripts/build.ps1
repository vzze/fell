param(
    [Parameter()]
    [Boolean]$BuildFell = $True,
    [Boolean]$BuildTests = $True
)

$check_dir = Test-Path ./build -PathType Container

if($check_dir -eq $false) {
    New-Item build -ItemType Directory
}

Write-Host "`n-------------- OPTS --------------`n"

Write-Host "BuildFell:  $BuildFell"
Write-Host "BuildTests: $BuildTests"

Write-Host "`n-------------- CMAKE --------------`n"

Invoke-Expression "cmake -S . -B build -G `"MinGW Makefiles`" -DBUILD_TESTS=$BuildTests -DBUILD_FELL=$BuildFell"
Invoke-Expression "cmake --build ./build"
