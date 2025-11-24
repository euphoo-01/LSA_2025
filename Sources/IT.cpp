#include <iostream>
#include "../stdafx.h"
#include "../Headers/IT.h"

namespace IT {
    IdTable Create(int size) {
        IdTable* idtable = new IdTable;
        if (size > TI_MAXSIZE) {
            throw ERROR_THROW(117);
        }
        idtable->maxsize = size;
        idtable->size = 0;
        idtable->table = new Entry[size];
        return *idtable;
    }

    void Add(IdTable& idtable, Entry entry) {
        if (idtable.size + 1 > idtable.maxsize) {
            throw ERROR_THROW(118);
        }
        idtable.table[idtable.size] = entry;
        idtable.size++;
    }

    Entry GetEntry(IdTable& idtable, int n) {
        if (n < 0 || n > idtable.size - 1) {
            throw ERROR_THROW(119);
        }
        return idtable.table[n];
    }
    int search(IdTable& idtable, IT::Entry entry, map<string, string> scop)
    {
        for (int j = 0; j < idtable.size; j++)
        {
            //cout << entry.id << " " << entry.scope_name << " " <<  idtable.table[j].id<< ' ' << idtable.table[j].scope_name << '\n';
            if (strcmp(entry.id, idtable.table[j].id) == 0 && entry.scope_name == idtable.table[j].scope_name)
            {
                return j;
            }
        }
        string buf = scop[entry.scope_name];
        while (buf != "") {
            for (int j = 0; j < idtable.size; j++)
            {
                //cout << entry.id << " " << entry.scope_name << " " <<  idtable.table[j].id<< ' ' << idtable.table[j].scope_name << '\n';
                if (strcmp(entry.id, idtable.table[j].id) == 0 && idtable.table[j].scope_name == buf)
                {
                    return j;
                }
            }
            buf = scop[buf];
        }
        return -1;
    }
    int search(IdTable& idtable, IT::Entry entry)
    {
        for (int j = 0; j < idtable.size; j++)
        {
            //cout << entry.id << " " << entry.scope_name << " " <<  idtable.table[j].id<< ' ' << idtable.table[j].scope_name << '\n';
            if (strcmp(entry.id, idtable.table[j].id) == 0 && entry.scope_name == idtable.table[j].scope_name)
            {
                return j;
            }
        }
        return -1;
    }

    void Delete(IdTable& idtable) {
        delete idtable.table;
        idtable.maxsize = 0;
        idtable.size = 0;
    }
}