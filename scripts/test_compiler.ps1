$files = Get-ChildItem "./tests/compiler" -Recurse

foreach($f in $files) {
    if($f.Extension -eq ".fell") {
        Invoke-Expression "./build/bin/fell $($f.FullName)"
    }
}