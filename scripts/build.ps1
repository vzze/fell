param(
    [ValidateSet("MinGW", "MSVC", "NONE")]
    [String]$compiler = "NONE",
    [ValidateSet("Release", "RelWithDebInfo", "Debug")]
    [String]$build = "RelWithDebInfo"
)

enum Toolset {
    MinGW = 0
    MSVC  = 1
    NONE  = 2
}

enum Build {
    Release        = 0
    RelWithDebInfo = 1
    Debug          = 2
}

$toolset_opts = [String[]](
    "MinGW Makefiles",
    "Visual Studio 17 2022"
)

$build_opts = [String[]](
    "Release",
    "RelWithDebInfo",
    "Debug"
)

[Toolset]$compiler = $compiler
[Build]$build = $build

$check_dir = Test-Path ./build -PathType Container

if($compiler -eq [Toolset]::NONE) {
    if($check_dir -eq $false) {
        Write-Host "Please generate a build type."
    } else {
        Invoke-Expression "cmake --build ./build"
    }

    return;
}

if($check_dir -eq $false) {
    New-Item build -ItemType Directory
} else {
    Remove-Item build -Recurse
    New-Item build -ItemType Directory
}

Invoke-Expression "cmake -DCMAKE_BUILD_TYPE=$($build_opts[$build]) -S . -B build -G `"$($toolset_opts[$compiler])`""
Invoke-Expression "cmake --build ./build"
