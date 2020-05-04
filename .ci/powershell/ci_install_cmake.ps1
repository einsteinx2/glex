# Fail on error
$ErrorActionPreference="Stop"

# Install and unpack CMake
$global:ProgressPreference = "SilentlyContinue"
Invoke-WebRequest -URI https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-win64-x64.zip -OutFile $Env:HOMEPATH\cmake-3.16.4-win64-x64.zip
Expand-Archive $Env:HOMEPATH\cmake-3.16.4-win64-x64.zip -DestinationPath "$Env:ProgramFiles"
Rename-Item "$Env:ProgramFiles\cmake-3.16.4-win64-x64" -NewName CMake
