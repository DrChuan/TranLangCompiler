#include "SymbolTableItem.h"

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
        ret = isFunc ? SymbolType::VOID_S : SymbolType::ERROR;
    if (isFunc) ret |= SymbolType::FUNC_S;
    if (isClass) ret |= SymbolType::CLASS_S;
    if (isArray) ret |= SymbolType::ARRAY_S;
    return ret;
}

void SymbolTableItem::print(int indent)
{
    
}

SymbolTable *SymbolTableItem::getSubTable() const
{
    return tbPointer;
}