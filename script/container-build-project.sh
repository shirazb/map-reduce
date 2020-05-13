#!/bin/bash

printf "\nBuilding all targets in project SBMapReduce...\n\n"

mkdir build && \
cmake -S . -B build && \
cmake --build build || exit 1

printf "\n\n... built all targets in project SBMapReduce.\n"
