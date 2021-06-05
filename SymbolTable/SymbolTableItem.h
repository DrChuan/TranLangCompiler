#ifndef __SYMBOL_TABLE_ITEM__
#define __SYMBOL_TABLE_ITEM__

#include "../utility.h"
#include "../y.tab.h"

enum SymbolType
{
    ZERO = 0, INT_S = 0x1, DOUBLE_S = 0x2, STRING_S = 0x4, USER_DEFINED_S = 0x8, FUNC_S = 0x10, CLASS_S = 0x20, ARRAY_S = 0x40, ERROR = 0x80, VOID_S = 0x100
};

int createSymbolType(int type, bool isFunc, bool isClass, bool isArray);

class SymbolTable;

class SymbolTableItem
{
public:
    SymbolTableItem() {}
    SymbolTableItem(int offset, int type, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), tbPointer(tbPointer) {}
    SymbolTableItem(int offset, int type, string refName, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), refName(refName), tbPointer(tbPointer) {}
    SymbolTableItem(int offset, int type, int extraInfo, SymbolTable *tbPointer=nullptr) : offset(offset), type(type), extraInfo(extraInfo), tbPointer(tbPointer) {}
    void print();
    SymbolTable *getSubTable() const;
    int getExtraInfo() const { return extraInfo; }
    int getOffset() const { return offset; }
    int getType() const { return type; }
private:
    int offset;
    int type;        // 由SymbolType中的枚举量按位取或得到
    string refName;  // 对于用户自定义类型，保存其自定义类型名称
    int extraInfo;   // 对于函数类型，保存参数个数。对于class类型，保存类的字节数
    SymbolTable *tbPointer;
};

#endif