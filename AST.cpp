#include "AST.h"

const string ASTNode::non_terminals[] = {"start", "program", "function", "type", "para_list", "para", "declaration", "statements", "statement", "dcl_statement", 
                            "if_statement", "loop_statement", "exp_statement", "initialize", "exp", "while_loop", "else_part", "literal", 
                            "return_statement", "func_call", "args", "lexp", "class_def", "class_items", "class_item"};
const string ASTNode::terminals[] = {"FUNC", "ENDFUNC", "ID", "INT", "DOUBLE", "STRING", "VOID", "EPSILON", "SEMI", "INIT", "IF", "ENDIF", "ELSE", "ELSIF", 
                        "WHILE", "ENDWHILE", "INT_LITERAL", "DOUBLE_LITERAL", "STRING_LITERAL", "RETURN", "CLASS", "ENDCLASS", "COMMA", "ASSIGN", "AND", "OR", "EQUAL", "NEQ", "GEQ", 
                        "LEQ", "GREATER", "LESS", "ADD", "SUB", "MUL", "DIV", "MOD", "NOT", "LP", "RP", "LB", "RB", "DOT"};

ASTNode::ASTNode(int symbol) : m_children(vector<ASTNode*>(0)) { this->symbol = symbol; val.type = NOTHING; }

ASTNode::ASTNode(int symbol, vector<ASTNode*> children) : m_children(children)
{
    cout << children.size();
    // for (int i = 0; i < children.size(); i++)
    //     addChild(children[i]);
    this->symbol = symbol;
    val.type = NOTHING;
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
    while(ret.size() != 0 && ret.back() == nullptr)
        ret.pop_back();
    return ret;
}

// void ASTNode::addChild(ASTNode *child)
// {
//     if (this->child == nullptr)
//     {
//         this->child = child;
//         return;
//     }
//     ASTNode *p = this->child;
//     while(p->sibling)
//         p = p->sibling;
//     p->sibling = child;
// }

void ASTNode::merge()
{
    if (m_children.size() < 2)
        return;
    ASTNode *q;
    ASTNode *p = m_children[1];
    m_children.pop_back();
    while(p->m_children.size() == 2)
    {
        m_children.push_back(p->m_children[0]);
        q = p;
        p = p->m_children[1];
        delete q;
    }
    m_children.push_back(p->m_children[0]);
    // ASTNode *p = child->sibling;
    // ASTNode *q = child;
    // while(p != nullptr)
    // {
    //     q->sibling = p->child;
    //     q = q->sibling;
    //     delete p;
    //     p = q->sibling;
    // }
}

void ASTNode::mergeIf()
{
    while(getLastChild()->is(else_part))
    {
        for(int i = 0; i < getLastChild()->m_children.size(); i++)
            m_children.push_back(getLastChild()->m_children[i]);
        delete getLastChild();
    }
}

ASTNode *ASTNode::simplify()
{
    // ASTNode *t = child;
    for (int i = 0; i < m_children.size(); i++)
    {
        ASTNode *t = m_children[i];
        if (!t) continue;
        if (t->symbol == program || t->symbol == statements || t->symbol == para_list || t->symbol == args || t->symbol == class_items)
            t->merge();
        else if (t->symbol == if_statement)
            t->mergeIf();
    }
    return this;
}

ASTNode *ASTNode::getChild(int id) const
{
    return m_children[id];
}

const vector<ASTNode*> &ASTNode::getChildren() const
{
    return m_children;
}

ASTNode *ASTNode::getLastChild() const
{
    if (m_children.size() == 0)
        return nullptr;
    return m_children[m_children.size() - 1];
}

int ASTNode::childCount() const
{
    return m_children.size();
}

int ASTNode::getSymbol() const
{
    return symbol;
}

int ASTNode::getIntValue() const
{
    return val.val.ival;
}

double ASTNode::getDoubleValue() const
{
    return val.val.dval;
}

string ASTNode::getStringValue() const
{
    return val.val.sval;
}

ValueType ASTNode::getNodeValueType() const
{
    return val.type;
}

bool ASTNode::is(int objSymbol) const
{
    return symbol == objSymbol;
}

void ASTNode::setType(ASTNode *src)
{
    val.type = src->val.type;
}

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
    cout << m_children.size();
    print();
    printf("\n");
    for (int i = 0; i < m_children.size(); i++)
        if (m_children[i])
            m_children[i]->printDirectory(depth + 1);
        else
        {
            for (int i = 0; i <= depth; i++)
                printf("- ");
            cout << "[nothing]\n";
        }
    // ASTNode *t = child;
    // while(t)
    // {
    //     t->printDirectory(depth + 1);
    //     t = t->sibling;
    // }
}

void AST::printDirectory()
{
    printf("\n############### Syntax Tree ##################\n");
    root->printDirectory(0);
    printf("############# Syntax Tree End ################\n\n");
}