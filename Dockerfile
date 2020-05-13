FROM ubuntu:latest

WORKDIR usr/src/map-reduce

RUN apt-get update && apt-get install -y \
        clang clangd                     \
        git                              \
        lldb lld                         \
        make                             \
        wget

# Installs cmake and adds its exes into PATH.
COPY script/container-install-build-tools.sh script/
RUN /bin/bash script/container-install-build-tools.sh
RUN rm script/container-install-build-tools.sh

# Copy all required files (everything else should be dockerignored).
COPY . .

ENV CXX clang++
RUN /bin/bash script/container-build-project.sh
