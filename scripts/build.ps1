param(
    [Parameter()]
    [Boolean]$BuildFell = $True,
    [Boolean]$BuildTests = $False,
    [Boolean]$RunTests = $False
)

$check_dir = Test-Path ./build -PathType Container

if($check_dir -eq $false) {
    New-Item build -ItemType Directory
}

Write-Host "`n-------------- OPTS --------------`n"

Write-Host "BuildFell:  $BuildFell"
Write-Host "BuildTests: $BuildTests"
Write-Host "RunTests:   $RunTests"

Write-Host "`n-------------- CMAKE --------------`n"

Invoke-Expression "cmake -S . -B build -G `"MinGW Makefiles`" -DBUILD_TESTS=$BuildTests -DBUILD_FELL=$BuildFell"
Invoke-Expression "cmake --build ./build"

if($RunTests -eq $True) {
    Write-Host "`n-------------- TESTS --------------`n"

    $files = Get-ChildItem "./build"

    foreach($f in $files) {
        if($f.Extension -eq ".exe") {
            Write-Host -NoNewLine "$($f.Name): "

            Invoke-Expression "$($f.FullName)"
        }
    }
}
