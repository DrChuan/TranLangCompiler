#ifndef __UTILITY__
#define __UTILITY__

#include <iostream>
#include <string>

using std::string;

// enum ValueType
// 结点中储存的值的类型。包括整形字面量、双精度浮点字面量、字符串字面量、标识符，以及没有值
enum ValueType{
    INT_L, DOUBLE_L, STRING_L, IDENTIFIER, NOTHING
};

// Value
// 结点中的值，包括一个union和一个指示union中类型的枚举量
class Value {
public:
    Value() {}
    union U {
    public:
        U() { new (&sval) string; }
        ~U() { sval.~string(); }
        int ival;
        double dval;
        string sval;
    } val;
    ValueType type;
};

class ASTNode;

typedef struct
{  
    ASTNode* node;
} YYSTYPE;

#endif