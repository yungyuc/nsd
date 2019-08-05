#!/bin/bash

export DEBIAN_FRONTEND=noninteractive

# Set up timezone.
sudo apt-get -q update
sudo apt-get -qy install tzdata
sudo ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime
sudo dpkg-reconfigure --frontend noninteractive tzdata

# Upgrade existing packages.
sudo apt-get -qy dist-upgrade

# Install building tools.
sudo apt-get -qy install sudo build-essential make cmake silversearcher-ag \
  libc6-dev gcc-7 g++-7 gcc-multilib

# Remove all trace of apt.
sudo rm -rf /var/lib/apt/lists/*

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

# Install miniconda
curl -sSL -o miniconda.sh \
  https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash miniconda.sh -b -p ${HOME}/opt/conda
rm -rf miniconda.sh

cat << EOF > ${HOME}/.bash_acct
namemunge PATH ${HOME}/opt/conda/bin
EOF

export PATH="${HOME}/opt/conda/bin:$PATH"

conda config --set channel_priority strict
conda update --all --yes
conda install --yes pip python numpy scipy pytest pandas matplotlib

pip install nbgitpuller sphinx-gallery notebook jupyterlab rise cxxfilt
pip install https://github.com/aldanor/ipybind/tarball/master

INSTALL_PREFIX=${INSTALL_PREFIX:-${HOME}/opt/conda}
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

xtensor_blas() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install QuantStack xtensor-blas "${cmakeargs[@]}"

}

xtensor_python() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install QuantStack xtensor-python "${cmakeargs[@]}"

}

pybind11
xtl
xsimd
xtensor
xtensor_blas
xtensor_python
