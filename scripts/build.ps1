param(
    [Parameter()]
    [String]$Type = "mingw"
)

$check_dir = Test-Path ./build -PathType Container

if($Type -eq "rebuild") {
    Invoke-Expression "cmake --build ./build --config Release"
    return
}

if($check_dir -eq $false) {
    New-Item build -ItemType Directory
} else {
    Remove-Item build -Recurse
    New-Item build -ItemType Directory
}

Write-Host "`n-------------- CMAKE --------------`n"

if($Type -eq "mingw") {
    Invoke-Expression "cmake -S . -B build -G `"MinGW Makefiles`""
} elseif($Type -eq "msvc") {
    Invoke-Expression "cmake -S . -B build -G `"Visual Studio 17 2022`""
} else {
    Write-Host "No -Type selected: mingw, msvc"
}

Invoke-Expression "cmake --build ./build --config Release"
