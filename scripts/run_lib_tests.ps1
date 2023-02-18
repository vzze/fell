Write-Host "`n-------------- LIB TESTS --------------"

$files = Get-ChildItem "./build"

foreach($f in $files) {
    if($f.Extension -eq ".exe" -and $f.Name -ne "fell.exe") {
        Write-Host "`n$($f.Name):`n"

        Invoke-Expression "$($f.FullName)"
    }
}
