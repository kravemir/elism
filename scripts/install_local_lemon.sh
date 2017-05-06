#!/usr/bin/env bash

LOCAL_DIR=${HOME}/.local

mkdir -p ${LOCAL_DIR}/bin ${LOCAL_DIR}/src
wget -q -O ${LOCAL_DIR}/bin/lempar.c http://www.sqlite.org/src/raw/tool/lempar.c?name=db1bdb4821f2d8fbd76e577cf3ab18642c8d08d1
wget -q -O ${LOCAL_DIR}/src/lemon.c http://www.sqlite.org/src/raw/tool/lemon.c?name=6b0cdffb377e79c6295c40300677688f3d35c21c0154da635642ef82dc3a5836
gcc ${LOCAL_DIR}/src/lemon.c -o ${LOCAL_DIR}/bin/lemon
chmod +x ${LOCAL_DIR}/bin/lemon

echo '# Set environment with new path:'
echo "export PATH=\"${LOCAL_DIR}/bin:\$PATH\""