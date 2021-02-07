#!/bin/bash
#
# Copyright (C) 2020 Yung-Yu Chen <yyc@solvcon.net>.
#

export DEBIAN_FRONTEND=noninteractive

if [ -z "$SKIP_APT" ] ; then
  # Set up timezone.
  sudo apt-get -q update
  sudo apt-get -qy install tzdata
  sudo ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime
  sudo dpkg-reconfigure --frontend noninteractive tzdata

  # Upgrade existing packages.
  sudo apt-get -qy dist-upgrade

  # Install building tools.
  sudo apt-get -qy install tmux build-essential make cmake silversearcher-ag \
    libc6-dev gcc-7 g++-7 gcc-multilib python

  # Remove all trace of apt.
  sudo rm -rf /var/lib/apt/lists/*
fi

if [ -z "$SKIP_HOME" ] ; then
  # Set up home directory.
  rm -rf workspace
  git clone git@github.com:yungyuc/workspace.git workspace
  rm -rf .git
  mv workspace/.git .
  rm -rf workspace
  git checkout -- .

  # Set up sub-directories in home.
  mkdir -p ${HOME}/tmp
  rm -rf ${HOME}/tmp/*
  mkdir -p ${HOME}/work/tmp
  rm -rf ${HOME}/work/tmp/*

  # Add NSD clone helper.
  cat << EOF > ${HOME}/work/clone-nsd.sh
git clone git@github.com:yungyuc/nsd.git \${HOME}/work/nsd
EOF
  chmod a+x work/clone-nsd.sh
fi

INSTALL_PREFIX=${INSTALL_PREFIX:-${HOME}/opt/conda}
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

if [ -z "$SKIP_INSTALL" ] ; then
  pybind11
  xtl
  xsimd
  xtensor
  xtensor_blas
  xtensor_python
fi
