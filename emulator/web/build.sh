#!/usr/bin/env bash
set -euo pipefail
cd /build
flags="-O2 -Wno-error=unused-command-line-argument -matomics -mbulk-memory -DNDEBUG -DG_DISABLE_ASSERT -D_GNU_SOURCE -sASYNCIFY=1 -pthread -sPROXY_TO_PTHREAD=1 -sFORCE_FILESYSTEM -sALLOW_TABLE_GROWTH -sINITIAL_MEMORY=512MB -sWASM_BIGINT -sMALLOC=mimalloc -sEXPORT_ES6=1 -sMODULARIZE=1 -sASYNCIFY_IMPORTS=ffi_call_js"
emconfigure /qemu/configure --static --target-list=arm-softmmu --with-devices-arm=wp2 \
  --cpu=wasm32 --cross-prefix= --without-default-features --without-default-devices \
  --enable-system --enable-tcg --enable-tcg-interpreter --with-coroutine=fiber --disable-werror \
  --disable-docs --disable-tools --disable-guest-agent --disable-plugins \
  --extra-cflags="$flags" --extra-cxxflags="$flags" \
  --extra-ldflags="-sEXPORTED_RUNTIME_METHODS=getTempRet0,setTempRet0,addFunction,removeFunction,FS,ENV"
emmake make -j2 qemu-system-arm
