FROM gcc:latest

# Installiamo Cppcheck (analizzatore statico) e Valgrind (check memoria)
RUN apt-get update && apt-get install -y cppcheck valgrind python3 nodejs && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copiamo il tester che lancia gli input casuali
COPY tester.cpp /tester.cpp
# Compiliamo il tester
RUN g++ -O3 /tester.cpp -o /engine

ENTRYPOINT ["/engine"]
