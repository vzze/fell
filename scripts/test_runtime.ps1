$files = Get-ChildItem "./tests/runtime" -Recurse

foreach($f in $files) {
    if($f.Extension -eq ".fell") {
        Invoke-Expression "./build/bin/fell $($f.FullName)"
    }
}
