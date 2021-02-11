#!/bin/bash
#
# Copyright (C) 2018-2021 Yung-Yu Chen <yyc@solvcon.net>.
#
# Build and install nsd dependencies.

# One-liner for provisioning an AWS instance:
# su ubuntu -c 'curl -s https://raw.githubusercontent.com/yungyuc/nsd/master/contrib/install.sh | bash -s setup_aws'

INSTALL_PREFIX=${INSTALL_PREFIX:-/usr}

if [ "$(uname)" == "Darwin" ] ; then
  NP=${NP:=$(sysctl -n hw.ncpu)}
elif [ "$(uname)" == "Linux" ] ; then
  NP=${NP:=$(cat /proc/cpuinfo | grep processor | wc -l)}
else
  NP=${NP:=1}
fi

NP=$(($NP*2))

enforce_linux() {

  if [ "$(uname)" != "Linux" ]; then
    echo "$0 must run on Linux, exiting"
    exit 1
  fi

}

run_apt() {

  enforce_linux

  # Set up timezone.
  sudo apt-get -q update
  sudo apt-get -qy install tzdata
  sudo ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime
  sudo dpkg-reconfigure --frontend noninteractive tzdata

  # Upgrade existing packages.
  sudo apt-get -qy dist-upgrade

  # Install building tools.
  sudo DEBIAN_FRONTEND=noninteractive apt-get -qy install \
    tmux build-essential make cmake silversearcher-ag \
    libc6-dev gcc-7 g++-7 gcc-multilib \
    gcc g++ gcc-10 g++-10 clang clang-tidy clang-10 clang-tidy-10 \
    gfortran gfortran-10 intel-mkl-full \
    python3 python3-pip python3-pytest \
    python3-numpy python3-scipy python3-pandas python3-matplotlib \
    jupyter

  # Remove all trace of apt.
  sudo rm -rf /var/lib/apt/lists/*

}

run_home() {

  enforce_linux

  # Set up home directory.
  rm -rf $HOME/workspace
  git clone https://github.com/yungyuc/workspace $HOME/workspace
  rm -rf $HOME/.git
  mv $HOME/workspace/.git $HOME
  rm -rf $HOME/workspace
  (cd $HOME ; git checkout -- .)

  # Set up sub-directories in home.
  mkdir -p ${HOME}/tmp
  rm -rf ${HOME}/tmp/*
  mkdir -p ${HOME}/work/tmp
  rm -rf ${HOME}/work/tmp/*

  # Add NSD clone helper.
  cat << EOF > ${HOME}/work/clone-nsd.sh
git clone git@github.com:yungyuc/nsd \${HOME}/work/nsd
EOF
  chmod a+x work/clone-nsd.sh

}

run_pip() {

  enforce_linux

  sudo pip3 install flake8 nbgitpuller sphinx-gallery notebook jupyterlab rise cxxfilt
  sudo pip3 install https://github.com/aldanor/ipybind/tarball/master

}

radare2() {

  mysudo="$1"
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
  ./configure --prefix=${INSTALL_PREFIX} ; ret=$?
  if [ $ret -ne 0 ] ; then echo "configure failure" ; exit 2 ; fi
  make -j $NP ; ret=$?
  if [ $ret -ne 0 ] ; then echo "make failure" ; exit 2 ; fi
  $mysudo make install -j $NP ; ret=$?
  if [ $ret -ne 0 ] ; then echo "make install failure" ; exit 2 ; fi
  popd
  $mysudo rm -rf $workdir

}

cmake_install() {

  mysudo=$1
  githuborg=$2
  pkgname=$3
  pkgbranch=$4
  pkgfull=$5
  cmakeargs="${@:6}"
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
  cmake $cmakeargs .. ; ret=$?
  if [ $ret -ne 0 ] ; then echo "cmake failure" ; exit 2 ; fi
  $mysudo make install -j $NP ; ret=$?
  if [ $ret -ne 0 ] ; then echo "make failure" ; exit 2 ; fi
  popd
  $mysudo rm -rf $workdir

}

pybind11() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DPYTHON_EXECUTABLE:FILEPATH=`which python3`")
  cmakeargs+=("-DPYBIND11_TEST=OFF")
  cmake_install sudo \
    ${PYBIND_ORG:-pybind} pybind11 ${PYBIND_BRANCH:-v2.6.2} \
    ${PYBIND_LOCAL:-pybind11-2.6.2} "${cmakeargs[@]}"

}

xtl() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmake_install sudo \
    ${XTL_ORG:-xtensor-stack} xtl ${XTL_BRANCH:-0.6.9} \
    ${XTL_LOCAL:-xtl-0.6.9} "${cmakeargs[@]}"

}

xsimd() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DBUILD_TESTS=OFF")
  cmake_install sudo \
    ${XSIMD_ORG:-xtensor-stack} xsimd ${XSIMD_BRANCH:-7.4.4} \
    ${XSIMD_LOCAL:-xsimd-7.4.4} "${cmakeargs[@]}"

}

xtensor() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmake_install sudo \
    ${XTENSOR_ORG:-xtensor-stack} xtensor ${XTENSOR_BRANCH:-0.21.3} \
    ${XTENSOR_LOCAL:-xtensor-0.21.3} "${cmakeargs[@]}"

}

xtensor_blas() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmake_install sudo \
    ${XTENSOR_BLAS_ORG:-xtensor-stack} xtensor-blas \
    ${XTENSOR_BLAS_BRANCH:-0.17.1} \
    ${XTENSOR_BLAS_LOCAL:-xtensor-blas-0.17.1} "${cmakeargs[@]}"

}

xtensor_python() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmake_install sudo \
    ${XTENSOR_PYTHON_ORG:-xtensor-stack} xtensor-python \
    ${XTENSOR_PYTHON_BRANCH:-0.24.1} \
    ${XTENSOR_PYTHON_LOCAL:-xtensor-python-0.24.1} "${cmakeargs[@]}"

}

setup_aws() {

  echo "SKIP_APT=${SKIP_APT}"
  if [[ "0" != "$SKIP_APT" ]] ; then
    run_apt
  else
    echo "skip apt"
  fi

  echo "SKIP_APT=${SKIP_HOME}"
  if [[ "0" != "$SKIP_HOME" ]] ; then
    run_home
  else
    echo "skip home"
  fi

  echo "SKIP_APT=${SKIP_PIP}"
  if [[ "0" != "$SKIP_PIP" ]] ; then
    run_pip
  else
    echo "skip pip"
  fi

}

everything_cmake() {

  pybind11
  xtl
  xsimd
  xtensor
  xtensor_blas
  xtensor_python

}

# Run function.
eval "$1"
