# --- STAGE 1: Compilazione ---
FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y g++ --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*
COPY tester.cpp /build/tester.cpp
RUN g++ -O3 /build/tester.cpp -o /engine

# --- STAGE 2: Runtime (Immagine finale) ---
FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# Installiamo TUTTI i tool che ti servono
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    valgrind \
    python3 \
    python3-pip \
    octave \
    chktex \
    clang-format \
    cppcheck \
    cmake \
    make \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Prendiamo l'engine già pronto dallo stage 1
COPY --from=builder /engine /usr/local/bin/engine
RUN chmod +x /usr/local/bin/engine

# IMPORTANTE PER GITHUB ACTIONS:
# GitHub monta il codice del repo in /github/workspace
WORKDIR /github/workspace

ENTRYPOINT ["engine"]