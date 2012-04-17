#bin/bash

rm -rf _build
mkdir  _build
cd _build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mudisp4  -DENABLE_DOCS=ON ../
make
