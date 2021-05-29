#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include <unordered_map>
#include <string>
#include "utility.h"
#include "y.tab.h"
using std::unordered_map;
using std::string;

enum SymbolType
{
    ZERO = 0, INT_S = 0x1, DOUBLE_S = 0x2, STRING_S = 0x4, USER_DEFINED_S = 0x8, FUNC_S = 0x10, CLASS_S = 0x20, ARRAY_S = 0x40, ERROR = 0x80
};

int createSymbolType(int type, bool isFunc, bool isClass, bool isArray);

class SymbolTable;

class SymbolTableItem
{
public:
    SymbolTableItem(int offset, int type, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), tbPointer(tbPointer) {}
    SymbolTableItem(int offset, int type, string refName, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), refName(refName), tbPointer(tbPointer) {}
    SymbolTableItem(int offset, int type, int extraInfo, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), extraInfo(extraInfo), tbPointer(tbPointer) {}
    int offset;
    int type;
    string refName;  // 对于用户自定义类型，保存其自定义类型名称
    int extraInfo;  // 对于函数类型，保存参数个数。对于数组类型，保存数组长度
    SymbolTable *tbPointer;
};

class SymbolTable
{
public:
    SymbolTable() {}
    bool insertItem(string name, SymbolTableItem item);
    SymbolTableItem *getItem(string name);
    bool deleteItem(string name);
private:
    unordered_map<string, SymbolTableItem> table;
};

#endif