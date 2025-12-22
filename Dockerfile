FROM gcc:latest

# Installiamo Python, Node, TypeScript e ora anche GNU Octave
RUN apt-get update && apt-get install -y \
    python3 \
    nodejs \
    npm \
    octave \
    && npm install -g typescript ts-node \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Prepariamo il tester C++
COPY tester.cpp /tester.cpp
RUN g++ -O3 /tester.cpp -o /engine

ENTRYPOINT ["/engine"]