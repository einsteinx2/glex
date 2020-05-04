# Fail on error
$ErrorActionPreference="Stop"

# Build the project
$BUILD_DIR="build"
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    New-Alias -Name cmake -Value "$Env:ProgramFiles\CMake\bin\cmake.exe"
}
New-Item -Name $BUILD_DIR -ItemType Directory
Push-Location $BUILD_DIR
cmake ..
cmake --build .
mv .\Debug\* .

# List the build artifacts
dir
Pop-Location
