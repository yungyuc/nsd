#!/bin/bash
#
# Copyright (C) 2018 Yung-Yu Chen <yyc@solvcon.net>.
#
# Build and install xtensor-python.

INSTALL_PREFIX=${INSTALL_PREFIX:-/usr}
INSTALL_VERSION=${INSTALL_VERSION:-master}

install() {

  githuborg=$1
  pkgname=$2
  cmakeargs="${@:3}"
  pkgbranch=${INSTALL_VERSION}
  pkgfull=$pkgname-$pkgbranch
  pkgrepo=https://github.com/$githuborg/$pkgname.git

  workdir=$(mktemp -d /tmp/build.XXXXXXXXX)
  echo "Work directory: $workdir"
  mkdir -p $workdir
  pushd $workdir
  curl -sSL -o $pkgfull.tar.gz \
    https://github.com/$githuborg/$pkgname/archive/$pkgbranch.tar.gz
  rm -rf $pkgfull
  tar xf $pkgfull.tar.gz
  cd $pkgfull
  mkdir -p build
  cd build
  cmake $cmakeargs ..
  make install
  popd
  rm -rf $workdir

}

pybind11() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DPYTHON_EXECUTABLE:FILEPATH=`which python3`")
  cmakeargs+=("-DPYBIND11_TEST=OFF")
  install pybind pybind11 "${cmakeargs[@]}"

}

xtl() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install QuantStack xtl "${cmakeargs[@]}"

}

xsimd() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DBUILD_TESTS=OFF")
  install QuantStack xsimd "${cmakeargs[@]}"

}

xtensor() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install QuantStack xtensor "${cmakeargs[@]}"

}

xtensor_python() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install QuantStack xtensor-python "${cmakeargs[@]}"

}

if [ $1 == "pybind11" ]; then
  pybind11
elif [ $1 == "xtl" ]; then
  xtl
elif [ $1 == "xsimd" ]; then
  xsimd
elif [ $1 == "xtensor" ]; then
  xtensor
elif [ $1 == "xtensor-python" ]; then
  xtensor_python
elif [ $1 == "everything" ]; then
  pybind11
  xtl
  xsimd
  xtensor
  xtensor_python
fi
