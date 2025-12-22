#include <iostream>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    
    // Questo codice crasha se n è 0 (divisione per zero)
    if (n == 0) {
        return 1; // Segnala errore al tester
    }
    
    std::cout << "Risultato: " << (100 / n) << std::endl;
    return 0;
}
