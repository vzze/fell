Write-Host "`n-------------- FELL TESTS --------------"

$files = Get-ChildItem "./tests"

foreach($f in $files) {
    if($f.Extension -eq ".fell") {
        Write-Host "`n$($f.Name):`n"

        Invoke-Expression "./build/fell $($f.FullName)"
    }
}
