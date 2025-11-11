## Firestarter 2
Inspired by the game from FLCL


## Why 2?
My goal is to expand on the concept, not just recreate the simple game from the anime. 

## Building

You may use either the make.sh script or emmake make to build and run the game.

### Dependencies
[raylib](https://github.com/raysan5/raylib) and [raylib-cpp](https://github.com/RobLoach/raylib-cpp) are both included as submodules as they need to be compiled for wasm. Using normal installations will not work.

[Emscripten](https://github.com/emscripten-core/emscripten) is required to build.

### Using make.sh

Instead of sourcing emsdk_env in my shell, I use the [make.sh](make.sh) script to source and execute so the modified `$PATH` does not escape.
If you want to use this script you will need to modify the script to use your emsdk_env location.

Otherwise you should just source the emsdk_env and then run make normally.

1. Dependencies
```./make.sh deps```

### Using emmake


## Credit
Font: [Spleen](https://github.com/fcambus/spleen)
