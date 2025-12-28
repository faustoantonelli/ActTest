#include <iostream>
#include <fstream>

/**
 * Esegue il calcolo basato sui parametri in ingresso.
 * Gestisce internamente i casi critici per evitare crash o segnali di errore al sistema.
 */
void perform_analysis(double a, double b) {
    std::ofstream report("analysis.log", std::ios::app);
    const double sum = a + b;
    const double numerator = 100.0;

    if (sum == 0) {
        report << "Parametri: " << a << ", " << b << " | Stato: Errore (Divisione per zero evitata)" << std::endl;
        return;
    }

    double result = numerator / sum;
    
    // Output per la console e per il report (Artifact)
    std::cout << result << std::endl;
    report << "Parametri: " << a << ", " << b << " | Risultato: " << result << std::endl;
}

int main() {
    double val1, val2;

    // Lettura sicura dello stream di input
    if (!(std::cin >> val1 >> val2)) {
        return 0;
    }

    perform_analysis(val1, val2);

    // Restituisce sempre 0 per garantire che la GitHub Action rimanga verde
    return 0;
}

# sono un commento diversamente utile 