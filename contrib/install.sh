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
  pkgbranch=$3
  pkgfull=$4
  cmakeargs="${@:5}"
  pkgrepo=https://github.com/$githuborg/$pkgname.git
  repotar=https://github.com/$githuborg/$pkgname/archive/$pkgbranch.tar.gz

  workdir=$(mktemp -d /tmp/build.XXXXXXXXX)
  echo "Work directory: $workdir"
  mkdir -p $workdir
  pushd $workdir
  echo "remote tarball: $repotar"
  curl -sSL -o $pkgfull.tar.gz $repotar
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
  install ${PYBIND_ORG:-pybind} pybind11 ${PYBIND_BRANCH:-v2.4.3} \
    ${PYBIND_LOCAL:-pybind11-2.4.3} "${cmakeargs[@]}"

}

xtl() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTL_ORG:-xtensor-stack} xtl ${XTL_BRANCH:-0.6.9} \
    ${XTL_LOCAL:-xtl-0.6.9} "${cmakeargs[@]}"

}

xsimd() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DBUILD_TESTS=OFF")
  install ${XSIMD_ORG:-xtensor-stack} xsimd ${XSIMD_BRANCH:-7.4.4} \
    ${XSIMD_LOCAL:-xsimd-7.4.4} "${cmakeargs[@]}"

}

xtensor() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTENSOR_ORG:-xtensor-stack} xtensor ${XTENSOR_BRANCH:-0.21.3} \
    ${XTENSOR_LOCAL:-xtensor-0.21.3} "${cmakeargs[@]}"

}

xtensor_blas() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTENSOR_BLAS_ORG:-xtensor-stack} xtensor-blas \
    ${XTENSOR_BLAS_BRANCH:-0.17.1} \
    ${XTENSOR_BLAS_LOCAL:-xtensor-blas-0.17.1} "${cmakeargs[@]}"

}

xtensor_python() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTENSOR_PYTHON_ORG:-xtensor-stack} xtensor-python \
    ${XTENSOR_PYTHON_BRANCH:-0.24.1} \
    ${XTENSOR_PYTHON_LOCAL:-xtensor-python-0.24.1} "${cmakeargs[@]}"

}

if [ $1 == "pybind11" ]; then
  pybind11
elif [ $1 == "xtl" ]; then
  xtl
elif [ $1 == "xsimd" ]; then
  xsimd
elif [ $1 == "xtensor" ]; then
  xtensor
elif [ $1 == "xtensor-blas" ]; then
  xtensor_blas
elif [ $1 == "xtensor-python" ]; then
  xtensor_python
elif [ $1 == "everything" ]; then
  pybind11
  xtl
  xsimd
  xtensor
  xtensor_blas
  xtensor_python
fi
