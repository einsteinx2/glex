#!/bin/bash

# Fail on error
set -e

# Build the project
. /opt/toolchains/dc/kos/environ.sh
./build_dc.sh

# List the build artifacts
ls -la ./build/dc
