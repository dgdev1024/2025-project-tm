#!/bin/bash

./tools/premake5 gmake
make -C generated/ $@ 2>&1 | tee build.log