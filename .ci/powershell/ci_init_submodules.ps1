# Fail on error
$ErrorActionPreference="Stop"

# Initialize the git submodules
git submodule init
git submodule update --remote
