#bin/bash

rm -rf _build_debug
mkdir  _build_debug
cd _build_debug	
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mudisp4 -DCMAKE_BUILD_TYPE=Debug -DENABLE_DOXYGEN=OFF  -DENABLE_UTILS=OFF ../
make

