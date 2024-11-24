#include <iostream>
#include <unistd.h>
#include <vector>
#include <cmath>

bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

void find_primes(int start, int end, int write_pipe) {
    for (int num = start; num < end; ++num) {
        if (is_prime(num)) {
            write(write_pipe, &num, sizeof(num));
        }
    }
    int end_signal = -1;
    write(write_pipe, &end_signal, sizeof(end_signal));
    close(write_pipe);
}

int main() {
    const int num_processes = 10;
    const int range = 1000;
    int pipes[num_processes][2];

    for (int i = 0; i < num_processes; ++i) {
        pipe(pipes[i]);
    }

    for (int i = 0; i < num_processes; ++i) {
        int start = i * range + 1;
        int end = (i + 1) * range + 1;

        if (fork() == 0) {
            close(pipes[i][0]);
            find_primes(start, end, pipes[i][1]);
            return 0;
        }
    }

    for (int i = 0; i < num_processes; ++i) {
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_processes; ++i) {
        int num;
        while (read(pipes[i][0], &num, sizeof(num)) > 0 && num != -1) {
            std::cout << num << std::endl;
        }
        close(pipes[i][0]);
    }

    return 0;
}
