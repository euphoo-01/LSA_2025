#include "../Headers/MFST.h"
#include <cstring>
#include <iostream>

namespace MFST {
    MfstState::MfstState() {
        lenta_position = 0;
        nrule = -1;
        nrulechain = -1;
    };

    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) {
        lenta_position = pposition;
        st = pst;
        nrulechain = pnrulechain;
    };

    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) {
        lenta_position = pposition;
        st = pst;
        nrule = pnrule;
        nrulechain = pnrulechain;
    };

    Mfst::MfstDiagnosis::MfstDiagnosis() {
        lenta_position = -1;
        rc_step = SURPRISE;
        nrule = -1;
        nrule_chain = -1;
    };

    Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) {
        lenta_position = plenta_position;
        rc_step = prc_step;
        nrule = pnrule;
        nrule_chain = pnrule_chain;
    };

    Mfst::Mfst() {
        lenta = 0;
        lenta_size = lenta_position = 0;
    };

    Mfst::Mfst(LT::LexTable& plex, GRB::Greibach pgreibach) {
        grebach = pgreibach;
        lex = plex;
        lenta = new GRBALPHABET[lenta_size = lex.size];
        for (int k = 0; k < lenta_size; k++)
            lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema[0]);
        lenta_position = 0;
        st.push(grebach.stbottomT);
        st.push(grebach.startN);
        nrulechain = -1;
    }

    Mfst::RC_STEP Mfst::step(std::ofstream& wr) {
        RC_STEP rc = SURPRISE;
        if (lenta_position < lenta_size) {
            if (GRB::Rule::Chain::isN(st.top())) {
                GRB::Rule rule;
                if ((nrule = grebach.getRule(st.top(), rule)) >= 0) {
                    GRB::Rule::Chain chain;
                    if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) {
                        MFST_TRACE1(wr);
                        savestate(wr); st.pop(); push_chain(chain); rc = NS_OK;
                        MFST_TRACE2(wr);
                    }
                    else {
                        MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", wr)
                        savediagnosis(NS_NORULECHAIN); rc = reststate(wr) ? NS_NORULECHAIN : NS_NORULE;
                    };
                }
                else rc = NS_ERROR;
            }
            else if ((st.top() == lenta[lenta_position])) {
                lenta_position++;
                st.pop();
                nrulechain = -1;
                rc = TS_OK;
                MFST_TRACE3(wr);
            }
            else {
                MFST_TRACE4("TS_NOK/NS_NORULECHAIN", wr) rc = reststate(wr) ? TS_NOK : NS_NORULECHAIN;
            };
        }
        else { rc = LENTA_END; MFST_TRACE4(LENTA_END, wr) };
        return rc;
    };

    bool Mfst::push_chain(GRB::Rule::Chain chain) {
        for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
        return true;
    };

    bool Mfst::savestate(std::ofstream& wr) {
        storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
        MFST_TRACE6("SAVESTATE:", storestate.size(), wr);
        return true;
    };

    bool Mfst::reststate(std::ofstream& wr) {
        bool rc = false;
        MfstState state;
        if (rc = (storestate.size() > 0)) {
            state = storestate.top();
            lenta_position = state.lenta_position;
            st = state.st;
            nrule = state.nrule;
            nrulechain = state.nrulechain;
            storestate.pop();
            MFST_TRACE5("RESSTATE", wr);
            MFST_TRACE2(wr);
        };
        return rc;
    };

    bool Mfst::savediagnosis(RC_STEP prc_step) {
        bool rc = false;
        short k = 0;
        while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position) k++;
        if (rc = (k < MFST_DIAGN_NUMBER)) {
            diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
            for (int j = k + 1; j < MFST_DIAGN_NUMBER; j++) diagnosis[j].lenta_position = -1;
        };
        return rc;
    };

    bool Mfst::start(std::ofstream& wr) {
        bool rc = false;
        RC_STEP rc_step = SURPRISE;
        char buf[MFST_DIAGN_MAXSIZE]{};
        rc_step = step(wr);
        while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
        {
            rc_step = step(wr);
        }

        switch (rc_step) {
        case LENTA_END: {
            MFST_TRACE4("------>LENTA_END", wr)
            wr << "------------------------------------------------------------------------------------------   ------\n";
            std::sprintf(buf, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
            wr << std::setw(4) << std::left << 0 << ": всего строк " << lex.table[lex.size - 1].sn << " , синтаксический анализ выполнен без ошибок\n";
            rc = true;
            break;
        }
        case NS_NORULE: {
            MFST_TRACE4("------>NS_NORULE", wr)
            wr << "------------------------------------------------------------------------------------------   ------\n";
            wr << getDiagnosis(0, buf) << '\n';
            wr << getDiagnosis(1, buf) << '\n';
            wr << getDiagnosis(2, buf) << '\n';
            break;
        }
        case NS_NORULECHAIN: MFST_TRACE4("------>NS_NORULECHAIN", wr) break;
        case NS_ERROR: MFST_TRACE4("------>NS_ERROR", wr) break;
        case SURPRISE: MFST_TRACE4("------>SURPRISE", wr) break;
        };
        return rc;
    };

    char* Mfst::getCSt(char* buf) {
        // Создаем временную копию стека для обхода
        MFSTSTSTACK temp_st = st;
        int i = 0;

        while (!temp_st.empty()) {
            short p = temp_st.top();
            temp_st.pop();
            buf[i++] = GRB::Rule::Chain::alphabet_to_char(p);
        }
        buf[i] = '\0';

        // Переворачиваем строку, так как стек выводится в обратном порядке
        for (int j = 0; j < i / 2; j++) {
            char temp = buf[j];
            buf[j] = buf[i - j - 1];
            buf[i - j - 1] = temp;
        }

        return buf;
    };

    char* Mfst::getCLenta(char* buf, short pos, short n) {
        short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
        for (i = pos; i < k; i++) buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
        buf[i - pos] = 0x00;
        return buf;
    };

    char* Mfst::getDiagnosis(short n, char* buf) {
        char* rc = new char[500] {};
        int errid = 0;
        int lpos = -1;
        if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0) {
            errid = grebach.getRule(diagnosis[n].nrule).iderror;
            Error::ERROr err = Error::geterror(errid);
            std::sprintf(buf, "%d: строка %d,%s", err.id, lex.table[lpos].sn, err.message);
            throw ERROR_THROW_IN(errid, lex.table[lpos].sn, 0);
            rc = buf;
        };
        return rc;
    };

    void Mfst::printrules(std::ofstream& wr) {
        // Создаем временную копию для обхода
        MFSTSTATE temp_store = storestate;
        std::vector<MfstState> states;

        // Переносим все состояния в вектор
        while (!temp_store.empty()) {
            states.push_back(temp_store.top());
            temp_store.pop();
        }

        // Обрабатываем в правильном порядке
        for (int k = states.size() - 1; k >= 0; k--) {
            MfstState state = states[k];
            GRB::Rule rule = grebach.getRule(state.nrule);
            MFST_TRACE7(wr);
        }
    }

    bool Mfst::savededucation() {
        // Создаем временную копию для обхода
        MFSTSTATE temp_store = storestate;
        std::vector<MfstState> states;

        // Переносим все состояния в вектор
        while (!temp_store.empty()) {
            states.push_back(temp_store.top());
            temp_store.pop();
        }

        deducation.size = states.size();
        deducation.nrules = new short[deducation.size];
        deducation.nrulechains = new short[deducation.size];

        for (unsigned short k = 0; k < states.size(); k++) {
            deducation.nrules[k] = states[states.size() - 1 - k].nrule;
            deducation.nrulechains[k] = states[states.size() - 1 - k].nrulechain;
        }
        return true;
    };
}