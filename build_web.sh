#!/bin/bash

docker run -it --rm -v $(pwd):$(pwd) \
    -w $(pwd) -u $(id -u):$(id -g) \
    emscripten/emsdk:2.0.12 \
    bash -c "mkdir -p build_web && cd build_web && emcmake cmake .. $@ && make -j$(nproc)"
