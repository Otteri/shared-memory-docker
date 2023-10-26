FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \ 
    build-essential \
    cmake \
    libopencv-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY CMakeLists.txt /app/CMakeLists.txt
COPY main.cpp /app/main.cpp

RUN /bin/bash -c "mkdir -p /app/build"
RUN /bin/bash -c "cd /app/build && cmake .. && make"


WORKDIR /app