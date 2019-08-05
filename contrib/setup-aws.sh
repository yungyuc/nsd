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

${HOME}/opt/conda/bin/conda config --set channel_priority strict
${HOME}/opt/conda/bin/conda update --all --yes
