FROM gcc:12

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        make \
		opencl-headers \
		ocl-icd-opencl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
