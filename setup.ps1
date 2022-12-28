# Cleanup any old Cmake artifacts
if( Test-Path -Path './build' ) {Remove-Item './build' -Recurse}

Write-Output "Generating Makefile..."
New-Item -Path './build' -ItemType Directory
Set-Location './build'
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/avr-gcc-toolchain.cmake"

exit 0