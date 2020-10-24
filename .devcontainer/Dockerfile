# [Choice] Ubuntu version: bionic, focal
ARG VARIANT=focal
FROM mcr.microsoft.com/vscode/devcontainers/base:${VARIANT}

# Update the VARIANT arg in devcontainer.json to pick an Ubuntu version: focal, bionic
ARG VARIANT="focal"
FROM buildpack-deps:${VARIANT}-curl

# Options for setup script
ARG INSTALL_ZSH="true"
ARG UPGRADE_PACKAGES="true"
ARG USERNAME=nsd
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Install needed packages and setup non-root user. Use a separate RUN statement to add your own dependencies.
COPY library-scripts/*.sh /tmp/library-scripts/
RUN \
  bash /tmp/library-scripts/common-debian.sh "${INSTALL_ZSH}" "${USERNAME}" "${USER_UID}" "${USER_GID}" "${UPGRADE_PACKAGES}" && \
    apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Location configuration.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qq update && \
  apt-get -qy install tzdata && \
  ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime && \
  dpkg-reconfigure --frontend noninteractive tzdata && \
  apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Install build packages.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qqy update && \
  apt-get -qy install --no-install-recommends \
    sudo curl git build-essential make cmake libc6-dev gcc-10 g++-10 \
    clang-tidy radare2 \
    python3 python3-pip python3-dev python3-venv && \
  apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Install pip3 pacakges.
RUN \
  pip3 install pip setuptools && \
  pip3 install numpy scipy pandas matplotlib pytest flake8 cython && \
  pip3 install nbgitpuller sphinx-gallery notebook jupyterlab rise cxxfilt && \
  pip3 install https://github.com/aldanor/ipybind/tarball/master

# Manual install packages.
RUN bash /tmp/library-scripts/install.sh everything && \
  rm -rf /var/lib/apt/lists/* /tmp/library-scripts

# Set up user and environment.
RUN echo 'nsd	ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER nsd
ENV HOME="/home/nsd"

# vim: set ff=unix ft=dockerfile et sw=2 ts=2 sts=2:
