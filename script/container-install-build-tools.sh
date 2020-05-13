#!/bin/bash

CMAKE_VER_MAJOR=cmake-3.17
CMAKE_VER=cmake-3.17.2-Linux-x86_64

mkdir install-cmake && \
cd install-cmake || exit 1

wget -q https://github.com/Kitware/CMake/releases/download/v3.17.2/${CMAKE_VER}.tar.gz && \
tar -zxvf ${CMAKE_VER}.tar.gz || exit 1

cd ${CMAKE_VER} && \
mv bin/* /usr/bin || exit 1

# What happens when share/aclocal etc. already exist in a future Ubuntu release;
# mv command will fail saying target directory not empty.
mv share/${CMAKE_VER_MAJOR} /usr/share && \
mv --no-clobber share/aclocal share/icons share/mime /usr/share && \
mv share/applications/* /usr/share/applications/* || exit 1

# cd back into script/
cd ../.. && \
rm -r install-cmake || exit 1
