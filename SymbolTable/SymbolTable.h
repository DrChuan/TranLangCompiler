#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include "../utility.h"
#include <unordered_map>
#include "SymbolTableItem.h"
using std::unordered_map;

class SymbolTable
{
public:
    SymbolTable() {}
    bool insertItem(string name, SymbolTableItem item);
    SymbolTableItem *getItem(string name);
    bool deleteItem(string name);
    int getSize() const { return table.size(); }

    void print();
private:
    unordered_map<string, SymbolTableItem> table;
    void _print(int indent);
};

#endif