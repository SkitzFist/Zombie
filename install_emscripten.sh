#!/bin/bash

# Download and compile xz utility
curl -O https://tukaani.org/xz/xz-5.2.5.tar.gz
tar -xzf xz-5.2.5.tar.gz
cd xz-5.2.5
./configure
make
cd ..
export PATH=$(pwd)/xz-5.2.5/src/xz:$PATH

# Download and install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest

# Make emsdk commands available for the current session
source ./emsdk_env.sh
