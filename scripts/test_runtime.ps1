$files = Get-ChildItem "./tests/runtime" -Recurse

foreach($f in $files) {
    if($f.Extension -eq ".fell") {
        Write-Output "`n$($f.Name):`n"
        Invoke-Expression "./build/bin/fell $($f.FullName)"
    }
}
