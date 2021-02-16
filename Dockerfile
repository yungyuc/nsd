FROM ubuntu:18.04

# Set up timezone.
RUN apt-get -q update \
    && apt-get install -y tzdata \
    && ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime \
    && dpkg-reconfigure --frontend noninteractive tzdata

# Upgrade existing packages.
RUN apt-get -qy dist-upgrade \
    && apt-get -qy install \
        sudo build-essential make cmake silversearcher-ag \
        libc6-dev gcc-7 g++-7 gcc-multilib

# Install building tools.
RUN apt-get -qy install sudo build-essential make cmake silversearcher-ag \
  libc6-dev gcc-7 g++-7 gcc-multilib \
  curl


# Install miniconda
RUN curl -SL -o miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh \
    && bash miniconda.sh -b -p /opt/conda \
    && rm -rf miniconda.sh
ENV PATH="/opt/conda/bin:${PATH}"

# update conda
RUN conda config --set channel_priority strict \
    && conda update --all --yes \
    && conda install --yes pip python numpy scipy pytest pandas matplotlib mkl-include

RUN pip install nbgitpuller sphinx-gallery notebook jupyterlab rise cxxfilt \
    && pip install https://github.com/aldanor/ipybind/tarball/master

ENV INSTALL_PREFIX="${INSTALL_PREFIX:-/opt/conda}"
ENV INSTALL_VERSION="${INSTALL_VERSION:-master}"

COPY contrib/install.sh /build/install.sh
WORKDIR /build
RUN ./install.sh everything

# user programs 
RUN apt-get -yq install git \
    && rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]
WORKDIR /nsd