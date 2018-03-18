#!/bin/sh
set -e
set -x

cmake -G "Unix Makefiles"
make clean && make
rm .gitattributes CMakeCache.txt cmake_install.cmake CMakeLists.txt eboot.bin LICENSE README.md Makefile VitaBackup VitaBackup.velf VitaBackup.vpk_param.sfo
rm -rf CMakeFiles include res sce_sys source tools 
