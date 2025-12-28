FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    python3 \
    octave \
    valgrind \
    chktex \
    clang-format \
    && rm -rf /var/lib/apt/lists/*

COPY tester.cpp /tester.cpp
RUN g++ -O3 /tester.cpp -o /engine

WORKDIR /app
ENTRYPOINT ["/engine"]