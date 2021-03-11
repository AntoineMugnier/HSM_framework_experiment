#include <iostream>

#include <chrono>

void run_qhsmtst();

int main() {
    for (int i = 0; i < 1000; ++i) {


     //  auto start = std::chrono::high_resolution_clock::now();

        run_qhsmtst();

     //   auto elapsed = std::chrono::high_resolution_clock::now() - start;

     //   long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

      //std::cout << "Operation took "<< microseconds << std::endl;
    }
}
