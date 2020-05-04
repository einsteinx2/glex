#!/bin/bash

# Fail on error
set -e

# Install the necessary package dependencies
sudo apt-get update
sudo apt-get install -y cmake xorg-dev
