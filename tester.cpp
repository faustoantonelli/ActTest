#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>
#include <algorithm>

void write_log(const std::string& message) {
    std::ofstream log_file("qa_report.log", std::ios::app);
    if (log_file.is_open()) log_file << message << std::endl;
}

std::string clean_output(std::string out) {
    std::stringstream ss(out);
    std::string line, result = "";
    while (std::getline(ss, line)) {
        if (line.find("==") == std::string::npos && !line.empty()) result += line + " ";
    }
    if (result.empty()) return "(null)";
    return (result.length() > 40) ? result.substr(0, 37) + "..." : result;
}

std::string generate_input(int count = 3) {
    static std::mt19937 rng(std::time(0));
    std::uniform_real_distribution<double> dist(-50.0, 50.0);
    std::stringstream ss;
    for (int i = 0; i < count; ++i) ss << std::fixed << std::setprecision(1) << dist(rng) << (i == count - 1 ? "" : " ");
    return ss.str();
}

int main(int argc, char* const argv[]) {
    if (argc < 2) return 1;
    std::string target = argv[1];
    std::string ext = target.substr(target.find_last_of(".") + 1);
    write_log("\n--- FILE: " + target + " ---");

    if (ext == "tex") {
        // --- 1. ERRORI ORTOGRAFICI ---
        write_log("--- ERRORI ORTOGRAFICI ---");
        std::system(("aspell list -t -l it < " + target + " | sort -u > tmp_sp.txt").c_str());
        std::ifstream sp_f("tmp_sp.txt"); std::string word; bool has_sp = false;
        while (sp_f >> word) {
            has_sp = true;
            std::string cmd = "echo " + word + " | aspell -a -l it | sed -n '2p' | cut -d: -f2 | cut -d, -f1 | xargs";
            FILE* pipe = popen(cmd.c_str(), "r"); char buffer[128]; std::string sug = "";
            if (fgets(buffer, 128, pipe)) sug = buffer; pclose(pipe);
            if (!sug.empty() && sug != " ") { sug.erase(sug.find_last_not_of(" \n\r\t")+1); write_log(word + " (-> correggi in: " + sug + ")"); }
        }
        if (!has_sp) write_log("Nessun errore rilevato.");

        // --- 2. COSA MANCA (LOG ANALYSIS) ---
        write_log("--- COSA MANCA (LOG ANALYSIS) ---");
        std::system(("pdflatex -interaction=batchmode -draftmode " + target + " >/dev/null 2>&1").c_str());
        std::string log_name = target.substr(0, target.find_last_of(".")) + ".log";
        std::system(("grep -E 'Warning: (Reference|Citation).*undefined' " + log_name + " > tmp_m.txt").c_str());
        std::ifstream m_f("tmp_m.txt"); std::string line; bool has_m = false;
        while (std::getline(m_f, line)) { write_log(line); has_m = true; }
        if (!has_m) write_log("Nessun riferimento mancante.");

        // --- 3. SUGGERIMENTI SINTASSI ---
        write_log("--- SUGGERIMENTI SINTASSI ---");
        std::system(("chktex -q -f 'Line %l: %m\\n' -n16 " + target + " > tmp_sy.txt").c_str());
        std::ifstream sy_f("tmp_sy.txt"); bool has_sy = false;
        while (std::getline(sy_f, line)) { write_log(line); has_sy = true; }
        if (!has_sy) write_log("Sintassi OK.");
        
        std::system("rm -f tmp_sp.txt tmp_m.txt tmp_sy.txt *.aux *.log *.out");
        return 0;
    }

    // Gestione Codice (C++, Python, Octave)
    std::string run_cmd;
    if (ext == "cpp") {
        if (std::system(("g++ -O3 " + target + " -o ./bin >/dev/null 2>&1").c_str()) != 0) {
            write_log("Note: Errore Compilazione"); return 1;
        }
        run_cmd = "timeout 2s valgrind --leak-check=full --error-exitcode=1 ./bin";
    } else if (ext == "py") { run_cmd = "timeout 2s python3 " + target; }
    else if (ext == "m") { run_cmd = "timeout 2s octave --no-gui --quiet " + target; }
    else { return 0; }

    for (int i = 1; i <= 10; ++i) {
        std::string input = generate_input(3);
        int status = std::system(("echo \"" + input + "\" | " + run_cmd + " > tmp.txt 2>&1").c_str());
        std::ifstream f("tmp.txt"); std::stringstream res; res << f.rdbuf();
        std::string out = clean_output(res.str());
        std::string note = (status == 124) ? "TIMEOUT" : (status != 0 ? "ERR/LEAK" : "OK");
        write_log("T" + std::to_string(i) + ") IN: [" + input + "] | OUT: [" + out + "] | Note: " + note);
        if (status != 0) break;
    }
    std::system("rm -f ./bin tmp.txt");
    return 0;
}