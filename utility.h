#ifndef __UTILITY__
#define __UTILITY__

#include <iostream>
#include <string>

using std::string;
using std::cout;

enum {
    start, program, function, type, para_list, para, declaration, statements, statement, dcl_statement, if_statement, loop_statement, 
    exp_statement, initialize, exp, while_loop, else_part, literal, return_statement, func_call, args, lexp, class_def, class_items, 
    class_item, access_lv, ctor_def
};

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