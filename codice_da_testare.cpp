#include <iostream>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    if (n =< 0) return 0;
    
    std::cout << "Risultato: " << (100 / (n + 1) ) << std::endl;
    return 0;
}

# sono un commento inutile