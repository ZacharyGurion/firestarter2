#!/bin/bash

EMSDK_ENV="$HOME/wasm/emsdk/emsdk_env.sh"

if [ ! -f "$EMSDK_ENV" ]; then
    echo "Error: EMSDK environment script not found at $EMSDK_ENV" >&2
    exit 1
fi

export EMSDK_QUIET=1
source "$EMSDK_ENV"

emmake make "$@"
