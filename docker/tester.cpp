#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Funzione di utilità per estrarre l'estensione del file
std::string get_ext(const std::string& f) {
    size_t pos = f.find_last_of(".");
    return (pos == std::string::npos) ? "" : f.substr(pos);
}

int main(int argc, char* argv[]) {
    // Verifica che sia stato passato un file come argomento
    if (argc < 2) {
        std::cerr << "❌ Errore: Nessun file specificato per il test." << std::endl;
        return 1;
    }

    std::string target = argv[1];
    std::string ext = get_ext(target);
    std::srand(std::time(0)); // Seed per i numeri casuali

    std::cout << "--- MONITORING SYSTEM: " << target << " ---" << std::endl;

    // --- 1. GESTIONE DOCUMENTI LATEX ---
    if (ext == ".tex") {
        std::cout << "🖋️ Rilevato file LaTeX. Avvio ChkTeX..." << std::endl;
        // -q (quiet), -n16 (ignora warning comuni sui prefissi)
        int res = std::system(("chktex -q -n16 " + target).c_str());
        if (res == 0) std::cout << "✅ Sintassi LaTeX corretta." << std::endl;
        else std::cout << "⚠️ ChkTeX ha rilevato avvisi o errori." << std::endl;
        return res;
    }

    // --- 2. GESTIONE CODICE (C++, Python, MATLAB/Octave) ---
    if (ext == ".cpp" || ext == ".py" || ext == ".m") {
        std::string cmd;

        // --- STEP A: ANALISI E PREPARAZIONE ---
        if (ext == ".cpp") {
            std::cout << "🛡️ Analisi statica Cppcheck..." << std::endl;
            std::system(("cppcheck --enable=all --suppress=missingIncludeSystem " + target).c_str());

            std::cout << "🔨 Compilazione C++..." << std::endl;
            if (std::system(("g++ -Wall " + target + " -o ./bin").c_str()) != 0) {
                std::cerr << "❌ Errore critico: Compilazione fallita!" << std::endl;
                return 1;
            }
            cmd = "./bin";
        } 
        else if (ext == ".py") {
            cmd = "python3 " + target;
        } 
        else if (ext == ".m") {
            cmd = "octave --quiet --no-gui " + target;
        }

        // --- STEP B: GENERAZIONE BATTERIA DI TEST ---
        std::vector<std::string> test_inputs = {"0", "1", "-1"}; // Casi critici
        for(int i = 0; i < 50; ++i) {
            // Genera numeri tra -1000 e 1000
            int r = (std::rand() % 2001) - 1000;
            test_inputs.push_back(std::to_string(r));
        }

        // --- STEP C: ESECUZIONE TEST ---
        std::cout << "🚀 Esecuzione di 53 scenari di test..." << std::endl;
        for (const auto& in : test_inputs) {
            // Mandiamo l'input tramite 'echo' e nascondiamo l'output standard per pulizia
            std::string full_cmd = "echo '" + in + "' | " + cmd + " > /dev/null 2>&1";
            int status = std::system(full_cmd.c_str());

            if (status != 0) {
                std::cerr << "🔴 FALLITO! Il codice ha crashato con input: " << in << std::endl;
                return 1; 
            }
        }

        std::cout << "✅ ECCELLENTE: Il codice ha superato tutti i test (50 casuali + 3 critici)." << std::endl;
        return 0;
    }

    // --- 3. FILE NON SUPPORTATO ---
    std::cout << "❓ Estensione " << ext << " non configurata per il debug automatico." << std::endl;
    return 0;
}