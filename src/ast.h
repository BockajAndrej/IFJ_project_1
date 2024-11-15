#ifndef AST_H
#define AST_H

// Typy uzlov
typedef enum {
    NODE_OP,       // operácia ( +, -, !=)
    NODE_VAR,      // premenna (a, b)
    NODE_CONST,    // constant (0, 1)
    NODE_IF,       // podmienka (if-else)
    NODE_WHILE,    // cyklus
    NODE_FUNC_DEF, //def. funkcie
    NODE_FUNC_CALL,//vloanie funkcie
    NODE_ORELSE,   //orelse funkcia
    NODE_BLOCK,    //blokc kodu
    NODE_ASSIGN,   // priradenie (a = b - a)
    NODE_RETURN,    // navratova
    NODE_PROG       //cely program ako koren
} NodeType;

// Štruktúra uzla AST
typedef struct ASTNode {
    NodeType type;          // typ uzla
    char* value;            // hodnota (napr. meno premennej alebo operátor)
    struct ASTNode* left;   // lavý potomok
    struct ASTNode* right;  // pravý potomok
    struct ASTNode** body;  // telo pre while/if
    int body_size;          // veľkosť tela (počet detí v zozname)
    struct ASTNode** params;// zoznam parametrov
    int params_size;        // pocet parametrov
} ASTNode;

// Funkcie pre AST
char *copy_string(const char *str);
void free_string(char *str);

ASTNode* createNode(NodeType type, const char* value);
ASTNode* createFunctionNode(const char* name);
ASTNode* createBlockNode();
void addParameters(ASTNode* func, ASTNode* param);
void addStatementToBlock(ASTNode* block, ASTNode* stmt);
void addChild(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* node);
void printAST(ASTNode* node, int indent);


#endif