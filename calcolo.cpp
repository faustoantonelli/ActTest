#include <iostream>


int main() {
    double a, b, c;
    // Legge i primi 3 numeri dei 20 inviati dal tester
    if (std::cin >> a >> b >> c) {
        std::cout << "Somma C++: " << (a + b + c) << std::endl;
    }
    return 0;
}
