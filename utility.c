#include "utility.h"
#include <stdlib.h>
#include <string.h>

ASTNode *createNode()
{
    ASTNode *ret = (ASTNode *)malloc(sizeof(ASTNode));
    ret->child = NULL;
    ret->sibling = NULL;
    return ret;
}

ASTNode *createNormalNode(int symbol, int nChild, ...)
{
    ASTNode *ret = createNode();
    ret->symbol = symbol;
    ret->val.type = NOTHING;
    va_list pArgs;
    va_start(pArgs, nChild);
    for (int i = 0; i < nChild; i++)
    {
        ASTNode *cur = va_arg(pArgs, ASTNode *);
        addChild(ret, cur);
    }
    return ret;
}

ASTNode *createIntNode(int symbol, int val, int nChild, ...)
{
    ASTNode *ret = createNormalNode(symbol, 0);
    ret->val.val.ival = val;
    ret->val.type = INT_L;
    va_list pArgs;
    va_start(pArgs, nChild);
    for (int i = 0; i < nChild; i++)
    {
        ASTNode *cur = va_arg(pArgs, ASTNode *);
        addChild(ret, cur);
    }
    return ret;
}

ASTNode *createDoubleNode(int symbol, double val, int nChild, ...)
{
    ASTNode *ret = createNormalNode(symbol, 0);
    ret->val.val.dval = val;
    ret->val.type = DOUBLE_L;
    va_list pArgs;
    va_start(pArgs, nChild);
    for (int i = 0; i < nChild; i++)
    {
        ASTNode *cur = va_arg(pArgs, ASTNode *);
        addChild(ret, cur);
    }
    return ret;
}

ASTNode *createStringNode(int symbol, char* val, int nChild, ...)
{
    ASTNode *ret = createNormalNode(symbol, 0);
    ret->val.val.sval = (char *)malloc(sizeof(char) * (strlen(val) + 1));
    val[strlen(val)-1] = 0;
    strcpy(ret->val.val.sval, val + 1);
    ret->val.type = STRING_L;
    va_list pArgs;
    va_start(pArgs, nChild);
    for (int i = 0; i < nChild; i++)
    {
        ASTNode *cur = va_arg(pArgs, ASTNode *);
        addChild(ret, cur);
    }
    return ret;
}

ASTNode *createIDNode(int symbol, char* val, int nChild, ...)
{
    ASTNode *ret = createNormalNode(symbol, 0);
    ret->val.val.sval = (char *)malloc(sizeof(char) * (strlen(val) + 1));
    strcpy(ret->val.val.sval, val);
    ret->val.type = IDENTIFIER;
    va_list pArgs;
    va_start(pArgs, nChild);
    for (int i = 0; i < nChild; i++)
    {
        ASTNode *cur = va_arg(pArgs, ASTNode *);
        addChild(ret, cur);
    }
    return ret;
}

void addChild(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL)
        printf("[Error in addChild] parent is null!\n");
    if (parent->child == NULL)
    {
        parent->child = child;
        return;
    }
    ASTNode *p = parent->child;
    while(p->sibling)
        p = p->sibling;
    p->sibling = child;
}

void _printPreorder(ASTNode *root)
{
    if (root == NULL)
        return;
    printf("%d ", root->symbol);
    _printPreorder(root->child);
    _printPreorder(root->sibling);
}

void printPreorder(ASTNode *root)
{
    printf("pre-order: ");
    _printPreorder(root);
    printf("\n");
}

void _printInorder(ASTNode *root)
{
    if (root == NULL)
        return;
    _printPreorder(root->child);
    printf("%d ", root->symbol);
    _printPreorder(root->sibling);
}

void printInorder(ASTNode *root)
{
    printf("in-order: ");
    _printInorder(root);
    printf("\n");
}

typedef struct {
    int front;
    int rear;
    ASTNode **elements;
} NodeQueue;

NodeQueue *createNodeQueue(int size)
{
    NodeQueue *ret = (NodeQueue *)malloc(sizeof(NodeQueue));
    ret->elements = (ASTNode **)malloc(sizeof(ASTNode *) * size);
    ret->front = 0;
    ret->rear = 0;
    return ret;
}

void enqueue(NodeQueue *queue, ASTNode *val)
{
    queue->elements[queue->rear++] = val;
}

ASTNode *dequeue(NodeQueue *queue)
{
    queue->front += 1;
    return queue->elements[queue->front-1];
}

int isEmpty(NodeQueue *queue)
{
    return queue->front == queue->rear;
}

char *non_terminals[] = {"start", "program", "function", "type", "para_list", "para", "declaration", "statements", "statement", "dcl_statement", 
                         "if_statement", "loop_statement", "exp_statement", "initialize", "exp", "while_loop", "else_part", "literal", 
                         "return_statement", "func_call", "args", "lexp", "class_def", "class_items", "class_item", "access", "ctor_def"};
char *terminals[] = {"FUNC", "ENDFUNC", "ID", "INT", "DOUBLE", "STRING", "VOID", "EPSILON", "SEMI", "INIT", "IF", "ENDIF", "ELSE", "ELSIF", 
                     "WHILE", "ENDWHILE", "INT_LITERAL", "DOUBLE_LITERAL", "STRING_LITERAL", "RETURN", "CLASS", "ENDCLASS", "PRIVATE", 
                     "PUBLIC", "PROTECTED", "INHERIT", "THIS", "CTOR", "ENDCTOR", "COMMA", "ASSIGN", "AND", "OR", "EQUAL", "NEQ", "GEQ", 
                     "LEQ", "GREATER", "LESS", "ADD", "SUB", "MUL", "DIV", "MOD", "NOT", "LP", "RP", "LB", "RB"};
int bias = 258;

void _printNode(ASTNode *node)
{
    if (node == NULL)
        printf("NULL ");
    else if (node->symbol == -1)
        printf(" | ");
    else if (node->child == NULL){
        printf("$%s", terminals[node->symbol - bias]);
        if (node->val.type == INT_L)
            printf("(%d)", node->val.val.ival);
        else if (node->val.type == DOUBLE_L)
            printf("(%lf)", node->val.val.dval);
        else if (node->val.type == STRING_L || node->val.type == IDENTIFIER)
            printf("(%s)", node->val.val.sval);
        printf(" ");
    }
    else
        printf("%s ", non_terminals[node->symbol]);
}

void printLayerorder(ASTNode *root)
{
    printf("\n############### Syntax Tree ##################\n");
    NodeQueue *queue1 = createNodeQueue(1000);
    NodeQueue *queue2 = createNodeQueue(1000);
    enqueue(queue1, root);
    ASTNode *t;
    while(!isEmpty(queue1) || !isEmpty(queue2))
    {
        while(!isEmpty(queue1))
        {
            t = dequeue(queue1);
            _printNode(t);
            if (t->child)
            {
                ASTNode *p = t->child;
                while(p)
                {
                    enqueue(queue2, p);
                    p = p->sibling;
                }
                enqueue(queue2, createNormalNode(-1, 0));
            }
        }
        printf("\n");
        while(!isEmpty(queue2))
        {
            t = dequeue(queue2);
            _printNode(t);
            if (t->child)
            {
                ASTNode *p = t->child;
                while(p)
                {
                    enqueue(queue1, p);
                    p = p->sibling;
                }
                enqueue(queue1, createNormalNode(-1, 0));
            }
        }
        printf("\n");
    }
    printf("############# Syntax Tree End ################\n\n");
}

void _printDirectory(ASTNode *node, int depth)
{
    if (node == NULL)
        return;
    for (int i = 0; i < depth; i++)
        printf("- ");
    _printNode(node);
    printf("\n");
    ASTNode *t = node->child;
    while(t)
    {
        _printDirectory(t, depth + 1);
        t = t->sibling;
    }
}

void printDirectory(ASTNode *root)
{
    printf("\n############### Syntax Tree ##################\n");
    _printDirectory(root, 0);
    printf("############# Syntax Tree End ################\n\n");
}