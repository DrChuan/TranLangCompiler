#include "SymbolTable.h"

int createSymbolType(int type, bool isFunc, bool isClass, bool isArray)
{
    int ret = SymbolType::ZERO;
    if (type == INT)
        ret = SymbolType::INT_S;
    else if(type == DOUBLE)
        ret = SymbolType::DOUBLE_S;
    else if(type == STRING)
        ret = SymbolType::STRING_S;
    else if(type == ID)
        ret = SymbolType::USER_DEFINED_S;
    else if (type == VOID)
        ret = isFunc ? SymbolType::ZERO : SymbolType::ERROR;
    if (isFunc) ret |= SymbolType::FUNC_S;
    if (isClass) ret |= SymbolType::CLASS_S;
    if (isArray) ret |= SymbolType::ARRAY_S;
    return ret;
}

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