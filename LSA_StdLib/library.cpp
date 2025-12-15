#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>

extern "C" {

    // вывод символа
    void lsa_writech(char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc < 128) {
            std::cout << c;
        } else {
            // конвертация cp1251 в utf-8 для кирилицы. поддержка linux консоли
            if (uc >= 0xC0 && uc <= 0xEF) {
                std::cout << (char)0xD0 << (char)(uc - 0x30);
            } else if (uc >= 0xF0 && uc <= 0xFF) {
                std::cout << (char)0xD1 << (char)(uc - 0x70);
            } else if (uc == 0xA8) { // Ё
                std::cout << (char)0xD0 << (char)0x81;
            } else if (uc == 0xB8) { // ё
                std::cout << (char)0xD1 << (char)0x91;
            } else {
                // другие символы cp1251
                std::cout << c; 
            }
        }
    }

    // вывод logic
    void lsa_writelogic(unsigned long long i) {
        if (i) std::cout << "true";
        else std::cout << "false";
    }

    // вывод числа
    void lsa_writeuint(unsigned long long i) {
        std::cout << i;
    }

    // чтение символа
    char lsa_readch() {
        char c;
        std::cout.flush();
        if (std::cin.get(c)) {
            return c;
        }
        return 0;
    }

    // переименование c++ функций для избежания конфликтов
    unsigned long long lsa_pow(unsigned long long b, unsigned long long e) {
        return static_cast<unsigned long long>(std::pow(b, e));
    }

    unsigned long long lsa_sqrt(unsigned long long x) {
         return static_cast<unsigned long long>(std::sqrt(x));
    }

    // получить минимум
    unsigned long long lsa_getMin(unsigned long long a, unsigned long long b) {
        return (a < b) ? a : b;
    }

    // получить максимум
    unsigned long long lsa_getMax(unsigned long long a, unsigned long long b) {
        return (a > b) ? a : b;
    }

    // проверка на простое число
    unsigned long long lsa_isPrime(unsigned long long n) {
        if (n <= 1) return 0;
        for (unsigned long long i = 2; i * i <= n; i++) {
            if (n % i == 0) return 0;
        }
        return 1;
    }

    // перевод в верхний регистр
    char lsa_toUpper(char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc >= 'a' && uc <= 'z') {
            return uc - 32;
        }
        // кириллица cp1251: нижний в верхний
        if (uc >= 0xE0 && uc <= 0xFF) {
            return uc - 32;
        }
        if (uc == 0xB8) { // ё
            return 0xA8; // Ё
        }
        return c;
    }
}