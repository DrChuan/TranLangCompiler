#include "SymbolTable.h"

bool SymbolTable::insertItem(string name, SymbolTableItem item)
{
    if (table.find(name) != table.end())
    {
        cout << "Error! Symbol " << name << " is already in this table!\n";
        return false;
    }
    table[name] = item;
    return true;
}

SymbolTableItem *SymbolTable::getItem(string name)
{
    if (table.find(name) == table.end())
    {
        cout << "Error! Symbol " << name << " does not exist in this table!\n";
        return nullptr;
    }
    return &table[name];
}

bool SymbolTable::deleteItem(string name)
{
    if (table.find(name) == table.end())
    {
        cout << "Error! Symbol " << name << " does not exist in this table!\n";
        return false;
    }
    table.erase(name);
    return true;
}

void SymbolTable::print()
{
    cout << "###############SymbolTable##################\n";
    _print(0);
    cout << "###############SymbolTable End##############\n\n";
}

void SymbolTable::_print(int indent)
{
    for (unordered_map<string, SymbolTableItem>::iterator i = table.begin(); i != table.end(); ++i)
    {
        for (int i = 0; i < indent; i++)
            cout << " ";
        cout << i->first << " ";
        i->second.print();
        cout << std::endl;
        if (i->second.getSubTable())
            i->second.getSubTable()->_print(indent + 2);
    }
}