#!/bin/bash

# Fail on error
set -e

# Initialize the git submodules
git submodule init
git submodule update --remote
