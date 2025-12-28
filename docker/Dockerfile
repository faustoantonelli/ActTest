# 1. Base: Immagine ufficiale GCC (Debian-based)
FROM gcc:latest

# Evita interazioni durante l'installazione
ENV DEBIAN_FRONTEND=noninteractive

# 2. Installazione strumenti di build, analisi, runtime e reportistica
RUN apt-get update && apt-get install -y --no-install-recommends \
    # Supporto Matlab/Octave (Aggiunto di nuovo)
    octave \
    # Automazione e Download
    cmake \
    ninja-build \
    git \
    ca-certificates \
    # Runtime e Scripting
    python3 \
    python3-pip \
    # Analisi Qualità (Static & Dynamic)
    cppcheck \
    clang-format \
    clang-tidy \
    valgrind \
    gdb \
    lcov \
    # Documentazione e Reportistica
    texlive-latex-base \
    texlive-fonts-recommended \
    latexmk \
    chktex \
    && rm -rf /var/lib/apt/lists/*

# 3. Configurazione cartella di lavoro
WORKDIR /app

# 4. Preparazione del motore di test universale
# Copiamo il tuo tester.cpp (assicurati che sia nella stessa cartella del Dockerfile)
COPY tester.cpp /tester.cpp
RUN g++ -O3 /tester.cpp -o /engine

# 5. Punto di ingresso
ENTRYPOINT ["/engine"]