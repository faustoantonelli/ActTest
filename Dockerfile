# Usa un'immagine con il compilatore C++ (GCC)
FROM gcc:latest

# Installa python3 e nodejs per supportare anche quegli script
RUN apt-get update && apt-get install -y python3 nodejs && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copia il motore di test dentro l'immagine
COPY tester.cpp /tester.cpp

# Compila il motore di test una volta sola durante la creazione
RUN g++ -O3 /tester.cpp -o /engine

# Quando il container si avvia, lancia il motore di test
ENTRYPOINT ["/engine"]