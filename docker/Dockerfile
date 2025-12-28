FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Installa C++, Python, Octave, Valgrind e ChkTex (per LaTeX)
RUN apt-get update && apt-get install -y \
    g++ \
    valgrind \
    python3 \
    octave \
    chktex \
    && rm -rf /var/lib/apt/lists/*

COPY tester.cpp /tester.cpp
RUN g++ -O3 /tester.cpp -o /engine

WORKDIR /app
ENTRYPOINT ["/engine"]