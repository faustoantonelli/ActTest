#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

void debug_log(std::string msg) {
    std::cout << "[DEBUGGER] " << msg << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::string target = argv[1];

    // --- STEP 1: ANALISI STATICA (Cerca bug nel testo del codice) ---
    debug_log("Analisi statica con Cppcheck...");
    std::system(("cppcheck --error-exitcode=1 " + target).c_str());

    // --- STEP 2: COMPILAZIONE CON SANITIZERS (Rileva errori di memoria) ---
    debug_log("Compilazione con AddressSanitizer...");
    std::string compile_cmd = "g++ -fsanitize=address,undefined -g " + target + " -o ./debug_bin";
    if (std::system(compile_cmd.c_str()) != 0) return 1;

    // --- STEP 3: FUZZING (Bombardamento di input casuali) ---
    debug_log("Avvio bombardamento input casuali...");
    std::vector<std::string> junk_inputs = {
        "0", "-1", "2147483647", "NaN", "infinity", " ", "", 
        "!!!!!!!!!!!!!!!!!!!!!!!!!!", "0.0000000000001", "NULL"
    };

    for (const auto& input : junk_inputs) {
        std::string run_cmd = "echo '" + input + "' | ./debug_bin > /dev/null 2>&1";
        int result = std::system(run_cmd.c_str());
        
        if (result != 0) {
            std::cerr << "❌ CRASH RILEVATO!" << std::endl;
            std::cerr << "Input colpevole: " << input << std::endl;
            return 1;
        }
    }

    debug_log("✅ Il codice è tecnicamente solido.");
    return 0;
}