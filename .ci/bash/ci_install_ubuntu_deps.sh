#!/bin/bash

# Fail on error
set -e

# Install the necessary package dependencies
apt-get update
apt-get install -y cmake xorg-dev
