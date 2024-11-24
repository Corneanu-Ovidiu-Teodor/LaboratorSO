#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>

// Funcție pentru a verifica dacă un număr este prim
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

// Funcție pentru a procesa intervalul într-un "proces"
void processInterval(int start, int end, std::vector<int>& primes) {
    for (int num = start; num <= end; ++num) {
        if (isPrime(num)) {
            primes.push_back(num);
        }
    }
}

int main() {
    const int NUM_PROCESSES = 10;
    const int RANGE = 10000;
    const int INTERVAL = RANGE / NUM_PROCESSES;

    std::vector<std::thread> threads;
    std::vector<std::vector<int>> results(NUM_PROCESSES);

    // Creăm "procesele" ca fire de execuție
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        int start = i * INTERVAL + 1;
        int end = (i + 1) * INTERVAL;
        threads.emplace_back(processInterval, start, end, std::ref(results[i]));
    }

    // Așteptăm toate firele să termine
    for (auto& t : threads) {
        t.join();
    }

    // Afișăm rezultatele
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        std::cout << "Proces " << i + 1 << ":\n";
        for (int prime : results[i]) {
            std::cout << prime << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
