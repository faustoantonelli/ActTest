# 1. Base: GCC per C++ moderno
FROM gcc:latest

# Evita interazioni durante l'installazione
ENV DEBIAN_FRONTEND=noninteractive

# 2. Installazione di tutti i motori di debug e runtime
RUN apt-get update && apt-get install -y \
    python3 \
    octave \
    # LaTeX e il suo debugger (ChkTeX)
    texlive-latex-base \
    chktex \
    # Strumenti di analisi "normali" per C++
    cppcheck \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

# 3. Configurazione cartella di lavoro
WORKDIR /app

# 4. Preparazione del motore di test universale
# Copiamo il tester.cpp che abbiamo aggiornato poco fa
COPY tester.cpp /tester.cpp
RUN g++ -O3 /tester.cpp -o /engine

# 5. Punto di ingresso: il container eseguirà il tester ogni volta
ENTRYPOINT ["/engine"]