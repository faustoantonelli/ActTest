#include <iostream>
#include <fstream>

/**
 * Calcola la somma dei primi due numeri e sottrae il terzo.
 * Salva il risultato nel file di log per gli Artifacts.
 */
void execute_simple_math(double a, double b, double c) {
    std::ofstream logger("analysis.log", std::ios::app);
    
    // Operazione richiesta: (a + b) - c
    double result = (a + b) - c;

    // Scrittura nel log per scaricarlo dagli Artifacts
    if (logger.is_open()) {
        logger << "Input: " << a << ", " << b << ", " << c 
               << " | Risultato: " << result << std::endl;
    }

    // Output a console per vedere i dati nei log di GitHub
    std::cout << "Calcolo terminato: " << result << std::endl;
}

int main() {
    double v1, v2, v3;

    // Lettura dei 3 numeri inviati dal tester
    if (!(std::cin >> v1 >> v2 >> v3)) {
        return 0;
    }

    execute_simple_math(v1, v2, v3);

    // Ritorna sempre 0: Action verde e Artifacts pronti
    return 0;
}