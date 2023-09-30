#!/bin/bash

# Download and extract xz utility
curl -L -o xz.tar.gz https://tukaani.org/xz/xz-5.2.5-linux.tar.gz
tar -xzf xz.tar.gz
# List the contents of the directory
ls -la $(pwd)/xz-5.2.5-linux/bin
# Give execution permissions to the binaries
chmod +x $(pwd)/xz-5.2.5-linux/bin/*
export PATH=$(pwd)/xz-5.2.5-linux/bin:$PATH

# Download and install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest

# Make emsdk commands available for the current session
source ./emsdk_env.sh
