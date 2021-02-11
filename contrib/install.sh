#!/bin/bash
#
# Copyright (C) 2018-2021 Yung-Yu Chen <yyc@solvcon.net>.
#
# Build and install nsd custom dependencies.

INSTALL_PREFIX=${INSTALL_PREFIX:-/usr}

if [ "$(uname)" == "Darwin" ] ; then
  NP=${NP:=$(sysctl -n hw.ncpu)}
elif [ "$(uname)" == "Linux" ] ; then
  NP=${NP:=$(cat /proc/cpuinfo | grep processor | wc -l)}
else
  NP=${NP:=1}
fi

NP=$(($NP*2))

radare2() {

  githuborg=radareorg
  pkgname=radare2
  pkgbranch=master
  pkgfull=radare2-master
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
  ./configure --prefix=${INSTALL_PREFIX}
  make -j $NP
  sudo make install -j $NP
  popd
  sudo rm -rf $workdir

}

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
  sudo make install -j $NP
  popd
  sudo rm -rf $workdir

}

pybind11() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DPYTHON_EXECUTABLE:FILEPATH=`which python3`")
  cmakeargs+=("-DPYBIND11_TEST=OFF")
  install ${PYBIND_ORG:-pybind} pybind11 ${PYBIND_BRANCH:-v2.6.2} \
    ${PYBIND_LOCAL:-pybind11-2.6.2} "${cmakeargs[@]}"

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

if [ $1 == "radare2" ]; then
  radare2
elif [ $1 == "pybind11" ]; then
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
  radare2
  pybind11
  xtl
  xsimd
  xtensor
  xtensor_blas
  xtensor_python
fi
