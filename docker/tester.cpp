#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

// Funzione che crea il "vassoio" di 20 numeri casuali
std::string generate_buffer_input(int count = 20) {
    std::stringstream ss;
    for (int i = 0; i < count; ++i) {
        double r = (std::rand() % 20001 - 10000) / 100.0;
        ss << std::fixed << std::setprecision(2) << r << " ";
    }
    return ss.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "ERRORE: Manca il file da testare." << std::endl;
        return 1;
    }

    std::string target = argv[1];
    std::srand(std::time(0));
    
    // Estrae l'estensione
    std::string ext = target.substr(target.find_last_of(".") + 1);
    std::string run_cmd;
    
    std::cout << "--- Avvio QA Engine su: " << target << " (" << ext << ") ---" << std::endl;

    // --- CASO 1: DOCUMENTI (LaTeX) ---
    if (ext == "tex") {
        std::cout << "📄 Modalità Documento: Controllo sintassi LaTeX..." << std::endl;
        // chktex verifica la sintassi senza compilare tutto il PDF (molto più veloce)
        int res = std::system(("chktex -q -n16 " + target).c_str());
        
        if (res == 0) {
            std::cout << "✅ LaTeX valido." << std::endl;
            return 0;
        } else {
            std::cerr << "❌ Errori di sintassi nel file LaTeX." << std::endl;
            return 1;
        }
    }

    // --- CASO 2: CODICE (C++, Python, Octave) ---
    if (ext == "cpp") {
        // Compila C++
        if (std::system(("g++ -O3 " + target + " -o ./test_bin").c_str()) != 0) return 1;
        // Esegue con Valgrind per trovare memory leak
        run_cmd = "valgrind --leak-check=full --error-exitcode=1 ./test_bin";
    } else if (ext == "py") {
        run_cmd = "python3 " + target;
    } else if (ext == "m") {
        // Octave senza interfaccia grafica
        run_cmd = "octave --no-gui --quiet " + target;
    } else {
        std::cerr << "Estensione non supportata: " << ext << std::endl;
        return 1;
    }

    // Loop di Stress Test (50 iterazioni)
    std::cout << "🔢 Modalità Calcolo: Avvio 50 test con input casuali..." << std::endl;
    for (int i = 1; i <= 50; ++i) {
        std::string input_data = generate_buffer_input(20);
        
        // Esegue il comando passando i numeri e nascondendo l'output (per pulizia)
        std::string full_cmd = "echo \"" + input_data + "\" | " + run_cmd + " > /dev/null 2>&1";
        
        if (std::system(full_cmd.c_str()) != 0) {
            std::cerr << "[FALLITO] Test #" << i << " su " << target << std::endl;
            return 1;
        }
    }

    std::cout << "[OK] Tutti i 50 test passati! ✅" << std::endl;
    return 0;
}