#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Funzione per estrarre l'estensione del file
std::string get_extension(const std::string& filename) {
    size_t last_dot = filename.find_last_of(".");
    if (last_dot == std::string::npos) return "";
    return filename.substr(last_dot);
}

int run_test(std::string cmd, int input_val) {
    // Usiamo una pipe per inviare l'input e nascondere l'output standard
    std::string full_cmd = "echo " + std::to_string(input_val) + " | " + cmd + " > /dev/null 2>&1";
    return std::system(full_cmd.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::string target = argv[1];
    std::string ext = get_extension(target);
    std::string cmd;

    // --- LOGICA MULTI-LINGUAGGIO ---
    if (ext == ".cpp") {
        std::cout << "🔨 Compilazione C++ in corso..." << std::endl;
        if (std::system(("g++ " + target + " -o ./user_bin").c_str()) != 0) return 1;
        cmd = "./user_bin";
    } 
    else if (ext == ".py") {
        cmd = "python3 " + target;
    } 
    else if (ext == ".js") {
        cmd = "node " + target;
    } 
    else if (ext == ".ts") {
        // Per TypeScript usiamo ts-node (che installeremo nel Dockerfile)
        cmd = "ts-node " + target;
    } 
    else if (ext == ".m") {
        // --quiet sopprime il messaggio di benvenuto, --no-gui evita l'interfaccia
        cmd = "octave --quiet --no-gui " + target;
    }
    else {
        std::cerr << "❌ Estensione " << ext << " non supportata!" << std::endl;
        return 1;
    }

    // Dataset di stress
    std::vector<int> inputs = {0, 1, -1, 1000, 2147483647, -2147483648};
    
    std::cout << "🚀 Testando: " << target << " con " << inputs.size() << " casi critici..." << std::endl;

    for (int val : inputs) {
        if (run_test(cmd, val) != 0) {
            std::cerr << "🔴 FALLITO! Crash con input: " << val << std::endl;
            return 1;
        }
    }

    std::cout << "🟢 ECCELLENTE! Il codice ha superato tutti i test." << std::endl;
    return 0;
}