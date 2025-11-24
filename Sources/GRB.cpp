#include "../Headers/GRB.h"
#include"../Headers/Rules.h"
#include "string.h"

namespace GRB {
    Rule::Chain::Chain(short psize, GRBALPHABET s, ...) {
        this->size = psize;
        this->nt = new GRBALPHABET[psize];
        this->nt[0] = s;  // Первый элемент уже передан

        if (psize > 1) {
            va_list args;
            va_start(args, s);
            for (short i = 1; i < psize; ++i) {
                this->nt[i] = va_arg(args, int);
            }
            va_end(args);
        }
    };

    Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) {
        this->nn = pnn;
        this->iderror = piderror;
        this->size = psize;
        this->chains = new Chain[psize];

        // Копируем первый chain (глубокое копирование)
        this->chains[0] = Chain();
        this->chains[0].size = c.size;
        this->chains[0].nt = new GRBALPHABET[c.size];
        for (short i = 0; i < c.size; i++) {
            this->chains[0].nt[i] = c.nt[i];
        }

        // Остальные chains через va_arg
        if (psize > 1) {
            va_list args;
            va_start(args, c);
            for (int i = 1; i < psize; i++) {
                Chain* chain_ptr = va_arg(args, Chain*);
                this->chains[i] = Chain();
                this->chains[i].size = chain_ptr->size;
                this->chains[i].nt = new GRBALPHABET[chain_ptr->size];
                for (short j = 0; j < chain_ptr->size; j++) {
                    this->chains[i].nt[j] = chain_ptr->nt[j];
                }
            }
            va_end(args);
        }
    };

    Rule::Chain::Chain(const Chain& other) {
        this->size = other.size;
        this->nt = new GRBALPHABET[this->size];
        for (short i = 0; i < this->size; i++) {
            this->nt[i] = other.nt[i];
        }
    }

    Rule::Chain& Rule::Chain::operator=(const Chain& other) {
        if (this != &other) {
            delete[] this->nt;
            this->size = other.size;
            this->nt = new GRBALPHABET[this->size];
            for (short i = 0; i < this->size; i++) {
                this->nt[i] = other.nt[i];
            }
        }
        return *this;
    }

    Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...) {
        this->startN = pstartN;
        this->stbottomT = pstbottom;
        this->rules = new Rule[this->size = psize];
        Rule* p = &r;
        for (int i = 0; i < size; i++) this->rules[i] = p[i];
    }

    short Greibach::getRule(GRBALPHABET pnn, Rule& prule) {
        short rc = -1;
        short k = 0;
        while (k < this->size && rules[k].nn != pnn) k++;
        if (k < this->size) prule = rules[rc = k];
        return rc;
    }

    Rule Greibach::getRule(short n) {
        Rule rc;
        if (n < this->size) rc = rules[n];
        return rc;
    };

    char* Rule::getCRule(char* b, short nchain) {
        char bchain[200];
        b[0] = Chain::alphabet_to_char(this->nn);
        b[1] = '-';
        b[2] = '>';
        b[3] = 0x00;
        this->chains[nchain].getCChain(bchain);
        std::strcat(b, bchain);
        return b;
    }

    short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) {
        short rc = -1;
        while (j < this->size && this->chains[j].nt[0] != t) ++j;
        rc = (j < this->size ? j : -1);
        if (rc >= 0) pchain = chains[rc];
        return rc;
    };

    char* Rule::Chain::getCChain(char* b) {
        for (int i = 0; i < this->size; i++) b[i] = Chain::alphabet_to_char(this->nt[i]);
        b[this->size] = '\0';
        return b;
    };

    Greibach getGreibach() {
        return greibach;
    }
}