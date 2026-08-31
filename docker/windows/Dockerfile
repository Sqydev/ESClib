FROM debian:stable-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        make \
        gcc-mingw-w64-x86-64 \
        opencl-headers \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /usr/x86_64-w64-mingw32/include \
    && cp -r /usr/include/CL /usr/x86_64-w64-mingw32/include/

WORKDIR /src
