FROM gurobi/optimizer:11.0.0 AS gurobi_ortools

WORKDIR /output/instances
WORKDIR /ajns

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update -yq \
    && apt-get -y install build-essential cmake lsb-release wget

RUN wget -q "https://github.com/google/or-tools/releases/download/v9.8/or-tools_amd64_ubuntu-20.04_cpp_v9.8.3296.tar.gz" \
    && tar -xzf or-tools_amd64_ubuntu-20.04_cpp_v9.8.3296.tar.gz \
    && rm or-tools_amd64_ubuntu-20.04_cpp_v9.8.3296.tar.gz \
    && mv or-tools_x86_64_Ubuntu-20.04_cpp_v9.8.3296/ ortools/

# RUN export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/ajns/ortools/lib/
# adicionar diretório que contém a lib do ortools no sistema
RUN echo "/ajns/ortools/lib/" >> /etc/ld.so.conf && ldconfig
RUN echo "/opt/gurobi/linux64/lib/" >> /etc/ld.so.conf && ldconfig

COPY . /ajns

ENV GRB_LICENSE_FILE /opt/gurobi/gurobi.lic

FROM gurobi_ortools 

WORKDIR /ajns/ajns_solver
RUN make all


CMD ["bash"]
# ENTRYPOINT [ "/ajns/ajns_solver/ajns.bin" ]



# RUN apt-get update -yq \
#     && apt-get -y install build-essential pkg-config git wget autoconf libtool zlib1g-dev gawk g++ \
#     curl make lsb-release python-dev gfortran libabsl-dev

# GCC 11
# RUN apt update -qq \
#     && apt install -yq software-properties-common \
#     && add-apt-repository ppa:ubuntu-toolchain-r/ppa -y \
#     && apt update \
#     && apt install -yq gcc-11 g++-11 \
#     && apt clean \
#     && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
# ENV CC=gcc-11 CXX=g++-11

# Install CMake v3.26.4
# RUN wget -q "https://cmake.org/files/v3.26/cmake-3.26.4-linux-x86_64.sh" \
#     && chmod a+x cmake-3.26.4-linux-x86_64.sh \
#     && ./cmake-3.26.4-linux-x86_64.sh --prefix=/usr/local/ --skip-license \
#     && rm cmake-3.26.4-linux-x86_64.sh

# FROM env AS devel
# WORKDIR /root
# RUN git clone -b "$SRC_GIT_BRANCH" --single-branch https://github.com/google/or-tools

# FROM devel AS build
# WORKDIR /root/or-tools
# RUN rm -rf build

# RUN cmake -S. -Bbuild -DBUILD_PYTHON=OFF -DBUILD_JAVA=OFF -DBUILD_DOTNET=OFF -DUSE_SCIP=OFF -DUSE_COINOR=OFF -DUSE_GUROBI=ON -DBUILD_DEPS=ON 
# RUN cmake --build build --target all -j16

# docker build . -t ajns --rm
# docker run -it --name ortools --rm ajns
# docker run -it --name ortools --rm ajns ./ajns.bin br17.12.txt 0 0 3 
# docker run -it --name ortools --volume=/mnt/a/Users/evellynsc/Documents/gurobi.lic:/opt/gurobi/gurobi.lic:ro --rm ajns ./ajns.bin br17.12.txt 0 0 3 
# docker run -it --name ortools --volume=/mnt/a/Users/evellynsc/Documents/gurobi.lic:/opt/gurobi/gurobi.lic:ro --volume=/mnt/a/Users/evellynsc/Documents/results_25_01_24:/output/instances --rm ajns ./run.sh

# https://organicprogrammer.com/2020/07/28/linux-cpp-docker/
# https://ddanilov.me/dockerized-cpp-build (boost)
# https://amytabb.com/tips/tutorials/2018/07/28/docker-tutorial-c-plus-plus/
# https://medium.com/codex/a-practical-guide-to-containerize-your-c-application-with-docker-50abb197f6d4
# https://awstip.com/c-docker-hello-world-f70a95e10cc1
