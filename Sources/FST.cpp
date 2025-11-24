#include <iostream>
#include <cstdarg>
#include <cstring>
#include "../Headers/FST.h"
using namespace std;

namespace FST {
    RELATION::RELATION(char c, short nn) {
        symbol = c;
        nnode = nn;
    }

    NODE::NODE() {
        n_relation = 0;
        relations = nullptr;
    }

    NODE::NODE(
        short n, // количество инцидентных ребер
        RELATION re1, ... // список ребер
    ) {
        this->n_relation = n;
        this->relations = new RELATION[n];
        va_list args;
        this->relations[0] = re1;
        va_start(args, n);
        for (int i = 1; i < n; i++) {
            this->relations[i] = va_arg(args, RELATION);
        }
        va_end(args);
    }

    FST::FST(
        const char *s, // цепочка
        short ns, // количество состояний автомата
        NODE n, ... // список состояний (граф переходов)
    ) {
        this->strin = s;
        this->position = 0;
        this->nstates = ns;
        this->nodes = new NODE[ns];
        this->nodes[0] = n;
        va_list args;
        va_start(args, ns);
        for (int i = 1; i < ns; i++) {
            this->nodes[i] = va_arg(args, NODE);
        }
        va_end(args);
        this->rstates = new short[ns];
        for (int i = 0; i < ns; i++) {
            this->rstates[i] = -1;
        }
    };

    bool step(FST &fst, short *&rstates) {
        bool rc = false;

        for (short i = 0; i < fst.nstates; i++) {
            if (rstates[i] == fst.position) {
                for (short j = 0; j < fst.nodes[i].n_relation; j++) {
                    if (fst.nodes[i].relations[j].symbol == fst.strin[fst.position]) {
                        rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
                        rc = true;
                    }
                }
            }
        }
        return rc;
    }

    bool execute(FST &fst) {
        short *rstates = new short[fst.nstates];
        memset(rstates, 0xff, sizeof(short) * fst.nstates);
        short lstring = strlen(fst.strin);
        bool rc = true;

        // Инициализация начального состояния
        rstates[0] = 0;

        for (short i = 0; i < lstring && rc; i++) {
            rc = step(fst, rstates);
            fst.position++;
        }

        bool result = (rc ? (rstates[fst.nstates - 1] == lstring) : rc);
        
        delete[] rstates;
        return result;
    }
}
