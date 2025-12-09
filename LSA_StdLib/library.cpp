#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>

extern "C" {

    void writech(char c) {
        std::cout << c;
    }

    void writeuint(unsigned long long i) {
        std::cout << i;
    }

    char readch() {
        char c;
        std::cin >> c;
        return c;
    }

    // Rename C++ functions to avoid conflict with std::pow/sqrt from headers
    // Use asm label to set the symbol name for the linker (what ASM calls)
    
    unsigned long long lsa_pow(unsigned long long b, unsigned long long e) asm("pow");
    unsigned long long lsa_pow(unsigned long long b, unsigned long long e) {
        return static_cast<unsigned long long>(std::pow(b, e));
    }

    unsigned long long lsa_sqrt(unsigned long long x) asm("sqrt");
    unsigned long long lsa_sqrt(unsigned long long x) {
         return static_cast<unsigned long long>(std::sqrt(x));
    }

    unsigned long long getMin(unsigned long long a, unsigned long long b) {
        return (a < b) ? a : b;
    }

    unsigned long long getMax(unsigned long long a, unsigned long long b) {
        return (a > b) ? a : b;
    }

    unsigned long long isPrime(unsigned long long n) {
        if (n <= 1) return 0;
        for (unsigned long long i = 2; i * i <= n; i++) {
            if (n % i == 0) return 0;
        }
        return 1;
    }

    char toUpper(char c) {
        return std::toupper(static_cast<unsigned char>(c));
    }
}