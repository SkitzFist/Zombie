#!/bin/bash

if[ -d "$HOME/emssdk"]; then
    echo "emsdk already installed"
else
    git clone https://github.com/emscripten-core/emsdk.git $HOME/emsdk
    cd $HOME/emsdk
    ./emsdk install latest
    ./emsdk activate latest
fi

source $HOME/emsdk/emsdk_env.sh