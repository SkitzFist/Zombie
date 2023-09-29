#!/bin/bash

# Download and install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest

# Make emsdk commands available for the current session
source ./emsdk_env.sh