#include "AST.h"

const string ASTNode::non_terminals[] = {"start", "program", "function", "type", "para_list", "para", "declaration", "statements", "statement", "dcl_statement", 
                            "if_statement", "loop_statement", "exp_statement", "initialize", "exp", "while_loop", "else_part", "literal", 
                            "return_statement", "func_call", "args", "lexp", "class_def", "class_items", "class_item", "access", "ctor_def"};
const string ASTNode::terminals[] = {"FUNC", "ENDFUNC", "ID", "INT", "DOUBLE", "STRING", "VOID", "EPSILON", "SEMI", "INIT", "IF", "ENDIF", "ELSE", "ELSIF", 
                        "WHILE", "ENDWHILE", "INT_LITERAL", "DOUBLE_LITERAL", "STRING_LITERAL", "RETURN", "CLASS", "ENDCLASS", "PRIVATE", 
                        "PUBLIC", "PROTECTED", "INHERIT", "THIS", "CTOR", "ENDCTOR", "COMMA", "ASSIGN", "AND", "OR", "EQUAL", "NEQ", "GEQ", 
                        "LEQ", "GREATER", "LESS", "ADD", "SUB", "MUL", "DIV", "MOD", "NOT", "LP", "RP", "LB", "RB"};

ASTNode::ASTNode(int symbol) : child(nullptr) { this->symbol = symbol; val.type = NOTHING; }

ASTNode::ASTNode(int symbol, vector<ASTNode*> children) : ASTNode(symbol)
{
    for (int i = 0; i < children.size(); i++)
        addChild(children[i]);
}

ASTNode::ASTNode(int symbol, int val, vector<ASTNode*> children) : ASTNode(symbol, children)
{
    this->val.type = ValueType::INT_L;
    this->val.val.ival = val;
}

ASTNode::ASTNode(int symbol, double val, vector<ASTNode*> children) : ASTNode(symbol, children)
{
    this->val.type = ValueType::DOUBLE_L;
    this->val.val.dval = val;
}

ASTNode::ASTNode(int symbol, string val, vector<ASTNode*> children, bool isID) : ASTNode(symbol, children)
{
    this->val.type = (isID ? ValueType::IDENTIFIER : ValueType::STRING_L);
    this->val.val.sval = (isID ? val : val.substr(1, val.size() - 1));
}

vector<ASTNode *> generateVector(int n, ...)
{
    vector<ASTNode *> ret(n);
    va_list pArgs;
    va_start(pArgs, n);
    for (int i = 0; i < n; i++)
        ret[i] = va_arg(pArgs, ASTNode *);
    va_end(pArgs);
    return ret;
}

void ASTNode::addChild(ASTNode *child)
{
    if (this->child == nullptr)
    {
        this->child = child;
        return;
    }
    ASTNode *p = this->child;
    while(p->sibling)
        p = p->sibling;
    p->sibling = child;
}

// void AST::printLayerorder(ASTNode *root)
// {
//     printf("\n############### Syntax Tree ##################\n");
//     NodeQueue *queue1 = createNodeQueue(1000);
//     NodeQueue *queue2 = createNodeQueue(1000);
//     enqueue(queue1, root);
//     ASTNode *t;
//     while(!isEmpty(queue1) || !isEmpty(queue2))
//     {
//         while(!isEmpty(queue1))
//         {
//             t = dequeue(queue1);
//             _printNode(t);
//             if (t->child)
//             {
//                 ASTNode *p = t->child;
//                 while(p)
//                 {
//                     enqueue(queue2, p);
//                     p = p->sibling;
//                 }
//                 enqueue(queue2, new ASTNode(-1));
//             }
//         }
//         printf("\n");
//         while(!isEmpty(queue2))
//         {
//             t = dequeue(queue2);
//             _printNode(t);
//             if (t->child)
//             {
//                 ASTNode *p = t->child;
//                 while(p)
//                 {
//                     enqueue(queue1, p);
//                     p = p->sibling;
//                 }
//                 enqueue(queue1, new ASTNode(-1));
//             }
//         }
//         printf("\n");
//     }
//     printf("############# Syntax Tree End ################\n\n");
// }

void ASTNode::print()
{
    if (symbol == -1)
        printf(" | ");
    else if (symbol >= bias){
        cout << "$" << terminals[symbol - bias];
        if (val.type == INT_L)
            printf("(%d)", val.val.ival);
        else if (val.type == DOUBLE_L)
            printf("(%lf)", val.val.dval);
        else if (val.type == STRING_L || val.type == IDENTIFIER)
            cout << "(" << val.val.sval << ")";
        printf(" ");
    }
    else
        cout << non_terminals[symbol];
}

void ASTNode::printDirectory(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("- ");
    print();
    printf("\n");
    ASTNode *t = child;
    while(t)
    {
        t->printDirectory(depth + 1);
        t = t->sibling;
    }
}

void AST::printDirectory()
{
    printf("\n############### Syntax Tree ##################\n");
    root->printDirectory(0);
    printf("############# Syntax Tree End ################\n\n");
}