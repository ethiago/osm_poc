FROM alpine:3.12.0

RUN apk add \ 
  build-base \
  cmake \
  mesa-dev \
  boost-dev \
  libpng-dev

RUN mkdir /app
COPY . /app
WORKDIR /app

RUN mkdir -p build/cmake && \
    cd build/cmake && \
    cmake ..
RUN cmake --build build/cmake

RUN bin/OSMPoc
