FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y g++ --no-install-recommends
WORKDIR /app
COPY tester.cpp .
RUN g++ -O3 tester.cpp -o /engine

FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ valgrind python3 octave chktex aspell aspell-it \
    texlive-latex-base texlive-lang-italian ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /engine /usr/local/bin/engine
RUN chmod +x /usr/local/bin/engine

WORKDIR /github/workspace
ENTRYPOINT ["engine"]

WORKDIR /github/workspace
ENTRYPOINT ["engine"]