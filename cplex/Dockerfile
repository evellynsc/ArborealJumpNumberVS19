# https://github.com/victorskl/docker-cplex/blob/master/Dockerfile
# https://leimao.github.io/blog/Boost-Docker/

FROM gcc:13.2.0

ARG BOOST_VERSION=1.80.0
ARG CMAKE_VERSION=3.25.1
ARG NUM_JOBS=8

ENV DEBIAN_FRONTEND noninteractive

WORKDIR /cplex

COPY "cos_installer_preview-22.1.1.0.R0-M08SWML-linux-x86-64.bin" .
COPY "response.properties" .

RUN chmod u+x "cos_installer_preview-22.1.1.0.R0-M08SWML-linux-x86-64.bin" && "./cos_installer_preview-22.1.1.0.R0-M08SWML-linux-x86-64.bin" -f "/cplex/response.properties"

WORKDIR /
RUN rm -rf "./cplex"

# Install package dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        software-properties-common \
        autoconf \
        automake \
        libtool \
        pkg-config \
        ca-certificates \
        libssl-dev \
        wget \
        git \
        curl \
        locales \
        locales-all \
        vim \
        gdb \
        valgrind && \
    apt-get clean

# System locale
# Important for UTF-8
ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8

# Install CMake
WORKDIR /tmp
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
    tar xzf cmake-${CMAKE_VERSION}.tar.gz && \
    cd cmake-${CMAKE_VERSION} && \
    ./bootstrap && \
    make -j${NUM_JOBS} && \
    make install && \
    rm -rf /tmp/*

# Install Boost
# https://www.boost.org/doc/libs/1_80_0/more/getting_started/unix-variants.html
WORKDIR /tmp
RUN BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*

CMD ["bash"]

# docker build . -t ajns:grb --rm
# docker run -it --name ortools --rm ajns
# docker run -it --name ortools --rm ajns ./ajns.bin br17.12.txt 0 0 3 
# docker run -it --name ortools --volume=/mnt/a/Users/evellynsc/Documents/gurobi.lic:/opt/gurobi/gurobi.lic:ro --rm ajns ./ajns.bin instances/small-br17.12.sop.ajn.txt 0 0 3 
# docker run -it --name ortools --rm ajns ./ajns.bin br17.12.txt 0 0 3
# docker run -it --name ortools --volume=/mnt/a/Users/evellynsc/Documents/gurobi.lic:/opt/gurobi/gurobi.lic:ro --rm ajns ./ajns.bin br17.12.txt 0 0 3
# docker run -it --name ortools --volume=/mnt/a/Users/evellynsc/Documents/gurobi.lic:/opt/gurobi/gurobi.lic:ro --volume=/mnt/a/Users/evellynsc/Documents/results_25_01_24:/output/instances --rm ajns ./run.sh


# docker tag local-image:tagname new-repo:tagname
# docker push new-repo:tagname
# docker run -i -t image/container /bin/bash.

# docker run -i -t --volume=/mnt/c/Users/evellynsc/source/repos/ArborealJumpNumber/cplex:/ajns/cplex ajns_cplex