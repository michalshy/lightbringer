#!/bin/bash

PROJECT_NAME="Lightbringer"
BUILD_DIR="build"
BIN_DIR="$BUILD_DIR/game/bin"
RES_DIR="res"

export_compdb() {
    premake5 export-compile-commands
}

generate() {
    premake5 gmake && export_compdb
}

build() {
    config_upper=$1      # Debug or Release (for folder names)
    config_lower=$(echo "$config_upper" | tr '[:upper:]' '[:lower:]') # debug or release

    echo "BUILDING $config_upper"

    make config=$config_lower

    mkdir -p "$BIN_DIR/$config_upper"
    cp -r "$RES_DIR" "$BIN_DIR/$config_upper/"
}

run_game() {
    config_upper=$1  # Debug or Release
    echo "Launching ($config_upper)"
    "$BIN_DIR/$config_upper/$PROJECT_NAME"
}

case $1 in
    generate) generate;;
    debug)       build "Debug";;
    release)     build "Release";;
    run-debug)   run_game "Debug";;
    run-release) run_game "Release";;
    *)
        echo "Usage:"
        echo "  $0 generate"
        echo "  $0 debug"
        echo "  $0 release"
        echo "  $0 run-debug"
        echo "  $0 run-release"
        ;;
esac
