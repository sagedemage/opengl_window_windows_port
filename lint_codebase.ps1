$cpp_files = Get-ChildItem -Path ..\src -Filter *.cpp -Recurse | ForEach-Object {
    "..\src\" + [System.IO.Path]::GetFileName($_)
}

$hpp_files = Get-ChildItem -Path ..\src -Filter *.hpp -Recurse | ForEach-Object {
    "..\src\" + [System.IO.Path]::GetFileName($_)
}

clang-tidy -format-style=file $cpp_files $hpp_files