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
    config=$1
    echo "BUILDING $config"
    make config=$config

    cp -r $RES_DIR "$BIN_DIR/$config/"
}

run_game() {
    config=$1
    echo "Launching"
    "$BIN_DIR/$config/$PROJECT_NAME"
}

case $1 in
    generate) generate;;
    debug) build "debug";;
    release) build "release";;
    run-debug) run_game "Debug";;
    run-release) run_game "Release";;
    *)
esac