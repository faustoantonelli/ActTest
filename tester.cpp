#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>

// Scrive i dettagli nel log per gli Artifacts
void write_log(const std::string& message) {
    std::ofstream log_file("qa_report.log", std::ios::app);
    if (log_file.is_open()) {
        log_file << message << std::endl;
    }
}

// Genera 20 numeri casuali per lo stress test
std::string generate_buffer_input(int count = 20) {
    static std::mt19937 rng(std::time(0));
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    std::stringstream ss;
    for (int i = 0; i < count; ++i) {
        ss << std::fixed << std::setprecision(2) << dist(rng) << " ";
    }
    return ss.str();
}

int main(int argc, char* const argv[]) { // 'const' aggiunto per cppcheck
    if (argc < 2) {
        std::cerr << "❌ ERRORE: Nessun file specificato." << std::endl;
        return 1;
    }

    std::string target = argv[1];
    std::string safe_target = "'" + target + "'";
    size_t last_dot = target.find_last_of(".");
    if (last_dot == std::string::npos) return 1;
    std::string ext = target.substr(last_dot + 1);

    // Inizializza il file di log
    std::ofstream log_init("qa_report.log");
    log_init << "--- REPORT QA ENGINE ---\nTarget: " << target << "\n" << std::endl;
    log_init.close();

    // --- FASE 1: ANALISI STATICA (C++) ---
    if (ext == "cpp") {
        std::cout << "🔍 Analisi Statica (cppcheck)..." << std::endl;
        // Suppress per i file di sistema e focus solo su Errori/Warning (no Style noise)
        std::string check_cmd = "cppcheck --enable=warning,error --suppress=missingIncludeSystem --error-exitcode=1 " + safe_target + " 2>> qa_report.log";
        if (std::system(check_cmd.c_str()) != 0) {
            std::cerr << "❌ Qualità codice insufficiente. Controlla qa_report.log" << std::endl;
            return 1;
        }
    }

    // --- FASE 2: PREPARAZIONE COMANDO ---
    std::string run_cmd;
    if (ext == "cpp") {
        std::cout << "🔨 Compilazione..." << std::endl;
        if (std::system(("g++ -O3 " + safe_target + " -o ./test_bin").c_str()) != 0) return 1;
        run_cmd = "timeout 5s valgrind --leak-check=full --error-exitcode=1 ./test_bin";
    } else if (ext == "py") {
        run_cmd = "timeout 5s python3 " + safe_target;
    } else if (ext == "m") {
        run_cmd = "timeout 5s octave --no-gui --quiet " + safe_target;
    } else if (ext == "tex") {
        std::cout << "📄 Controllo LaTeX..." << std::endl;
        return std::system(("chktex -q -n16 -I " + safe_target).c_str());
    } else {
        return 1;
    }

    // --- FASE 3: STRESS TEST (50 Iterazioni) ---
    std::cout << "🧪 Avvio Stress Test..." << std::endl;
    for (int i = 1; i <= 50; ++i) {
        std::string input_data = generate_buffer_input(20);
        std::string full_cmd = "echo \"" + input_data + "\" | " + run_cmd + " > /dev/null 2>&1";
        
        int status = std::system(full_cmd.c_str());
        if (status != 0) {
            write_log("❌ FALLITO Test #" + std::to_string(i));
            write_log("Input: " + input_data);
            if (status == 124) write_log("Motivo: Timeout (Possibile loop infinito)");
            else write_log("Motivo: Crash o Memory Leak");
            
            std::cerr << "❌ Fallito test #" << i << std::endl;
            return 1;
        }
        if (i % 10 == 0) std::cout << "   " << i << "/50 passati..." << std::endl;
    }

    if (ext == "cpp") std::system("rm ./test_bin");
    std::cout << "✅ Tutti i test completati con successo!" << std::endl;
    return 0;
}