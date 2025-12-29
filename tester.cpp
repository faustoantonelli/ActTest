#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>

void write_log(const std::string& message) {
    std::ofstream log_file("qa_report.log", std::ios::app);
    if (log_file.is_open()) log_file << message << std::endl;
}

std::string generate_buffer_input(int count = 20) {
    static std::mt19937 rng(std::time(0));
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    std::stringstream ss;
    for (int i = 0; i < count; ++i) {
        ss << std::fixed << std::setprecision(2) << dist(rng) << " ";
    }
    return ss.str();
}

int main(int argc, char* const argv[]) { // FIX: const aggiunto qui
    if (argc < 2) return 1;

    std::string target = argv[1];
    std::string safe_target = "'" + target + "'";
    std::string ext = target.substr(target.find_last_of(".") + 1);

    std::ofstream log_init("qa_report.log");
    log_init << "--- REPORT QA ENGINE ---\nTarget: " << target << "\n" << std::endl;
    log_init.close();

    if (ext == "cpp") {
        std::cout << "🔍 Analisi Statica (cppcheck)..." << std::endl;
        // FIX: --enable=warning,error riduce il rumore dei consigli estetici
        std::string check_cmd = "cppcheck --enable=warning,error --suppress=missingIncludeSystem --error-exitcode=1 " + safe_target + " 2>> qa_report.log";
        if (std::system(check_cmd.c_str()) != 0) {
            std::cerr << "❌ Qualità codice insufficiente (vedi qa_report.log)" << std::endl;
            return 1;
        }
    }

    std::string run_cmd;
    if (ext == "cpp") {
        if (std::system(("g++ -O3 " + safe_target + " -o ./test_bin").c_str()) != 0) return 1;
        run_cmd = "timeout 5s valgrind --leak-check=full --error-exitcode=1 ./test_bin";
    } else if (ext == "py") {
        run_cmd = "timeout 5s python3 " + safe_target;
    } else if (ext == "m") {
        run_cmd = "timeout 5s octave --no-gui --quiet " + safe_target;
    } else if (ext == "tex") {
        return std::system(("chktex -q -n16 -I " + safe_target).c_str());
    }

    std::cout << "🧪 Avvio Stress Test..." << std::endl;
    for (int i = 1; i <= 50; ++i) {
        std::string input_data = generate_buffer_input(20);
        std::string full_cmd = "echo \"" + input_data + "\" | " + run_cmd + " > /dev/null 2>&1";
        
        if (std::system(full_cmd.c_str()) != 0) {
            write_log("❌ FALLITO Test #" + std::to_string(i) + "\nInput: " + input_data);
            std::cerr << "❌ Fallito test #" << i << std::endl;
            return 1;
        }
    }

    if (ext == "cpp") std::system("rm ./test_bin");
    std::cout << "✅ QA Completato con successo!" << std::endl;
    return 0;
}