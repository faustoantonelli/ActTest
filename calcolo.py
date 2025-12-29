import sys

def main():
    # Legge l'input dallo standard input
    dati = sys.stdin.read().split()
    if len(dati) >= 3:
        a, b, c = map(float, dati[:3])
        print(f"Somma Python: {a + b + c}")

if __name__ == "__main__":
    main()
