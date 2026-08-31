FROM gcc:11

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        make \
        python3-pip \
        opencl-headers \
        ocl-icd-opencl-dev \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install --no-cache-dir cmake

WORKDIR /src
