#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// Esegue il comando e invia l'input tramite pipe
int run_test(std::string cmd, int input_val) {
    std::string full_cmd = "echo " + std::to_string(input_val) + " | " + cmd + " > /dev/null 2>&1";
    return std::system(full_cmd.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::string target = argv[1];
    std::string cmd;

    // Se è C++, dobbiamo prima compilarlo
    if (target.find(".cpp") != std::string::npos) {
        std::system(("g++ " + target + " -o ./user_bin").c_str());
        cmd = "./user_bin";
    } else if (target.find(".py") != std::string::npos) {
        cmd = "python3 " + target;
    } else if (target.find(".js") != std::string::npos) {
        cmd = "node " + target;
    }

    // Dataset di numeri "pericolosi"
    std::vector<int> inputs = {0, 1, -1, 100, -100, 2147483647, -2147483648};
    
    // Aggiungiamo 50 numeri casuali
    std::srand(std::time(0));
    for(int i=0; i<50; ++i) inputs.push_back(std::rand() % 20000 - 10000);

    std::cout << "🧪 Stress Test su: " << target << std::endl;

    for (int val : inputs) {
        if (run_test(cmd, val) != 0) {
            std::cerr << "❌ CRASH! Input che ha rotto il codice: " << val << std::endl;
            return 1;
        }
    }

    std::cout << "✅ Tutti i test passati!" << std::endl;
    return 0;
}