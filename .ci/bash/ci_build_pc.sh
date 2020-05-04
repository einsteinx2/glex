#!/bin/bash

# Fail on error
set -e

# Build the project
./build_pc.sh

# List the build artifacts
ls -la ./build/pc
