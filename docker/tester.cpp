#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string get_ext(const std::string& f) {
    size_t pos = f.find_last_of(".");
    return (pos == std::string::npos) ? "" : f.substr(pos);
}

std::string generate_random_input(int num_vars) {
    std::stringstream ss;
    for (int i = 0; i < num_vars; ++i) {
        double r = (std::rand() % 20001 - 10000) / 100.0; 
        ss << std::fixed << std::setprecision(2) << r << (i == num_vars - 1 ? "" : " ");
    }
    return ss.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "::error::Uso: ./engine <file> [num_vars]" << std::endl;
        return 1;
    }

    std::string target = argv[1];
    int num_vars = (argc >= 3) ? std::stoi(argv[2]) : 1;
    std::string ext = get_ext(target);
    std::srand(std::time(0));

    std::cout << "::group::🚀 QA Engine: " << target << " (" << num_vars << " vars)" << std::endl;

    std::string run_cmd;
    if (ext == ".cpp") {
        if (std::system(("g++ -O3 " + target + " -o ./bin_test").c_str()) != 0) return 1;
        run_cmd = "valgrind --leak-check=full --error-exitcode=1 ./bin_test";
    } else if (ext == ".py") {
        run_cmd = "python3 " + target;
    } else if (ext == ".m") {
        run_cmd = "octave --quiet --no-gui " + target;
    } else if (ext == ".tex") {
        std::cout << "::endgroup::" << std::endl;
        return std::system(("chktex -q -n16 " + target).c_str());
    }

    for (int i = 1; i <= 50; ++i) {
        std::string input_data = generate_random_input(num_vars);
        std::string full_cmd = "echo \"" + input_data + "\" | " + run_cmd + " > /dev/null 2>&1";
        
        if (std::system(full_cmd.c_str()) != 0) {
            std::cout << "::endgroup::" << std::endl;
            std::cerr << "::error file=" << target << "::Fallimento test #" << i << " con input: [" << input_data << "]" << std::endl;
            return 1;
        }
    }

    std::cout << "::endgroup::" << std::endl;
    std::cout << "✅ QA Passata con successo." << std::endl;
    return 0;
}