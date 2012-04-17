#bin/bash

rm -rf _build
rm -rf _build_debug
mkdir  _build_debug
mkdir  _build
cd _build_debug	
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mudisp4 -DCMAKE_BUILD_TYPE=Debug  ../
make
cd ../
cd _build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mudisp4  -DENABLE_DOCS=ON ../
make
