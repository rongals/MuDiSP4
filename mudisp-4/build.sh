#bin/bash

rm -rf _build
mkdir  _build
cd _build	
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mudisp4 -DCMAKE_BUILD_TYPE=Debug -DENABLE_DOCS=ENABLE_DOCS ../
make

