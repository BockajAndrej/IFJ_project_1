#ifndef _EXPRESSION_H
#define _EXPRESSION_H

/**
 * @enum Rules used for parsing.
 */
typedef enum
{
    N_DOLLAR,    /// $
    N_EQ_N,    /// E -> E == E  -1
    N_NEQ_N,   /// E -> E != E  -2
    N_LEQ_N,   /// E -> E <= E  -3
    N_LTN_N,   /// E -> E < E   -4
    N_MEQ_N,   /// E -> E >= E  -5
    N_MTN_N,   /// E -> E > E   -6
    N_PLUS_N,  /// E -> E + E   -7
    N_MINUS_N, /// E -> E - E   -8
    N_MUL_N,   /// E -> E * E   -9
    N_IDIV_N,  /// E -> E \ E   -10
    N_DIV_N,   /// E -> E / E   -11
    LB_N_RB,   /// E -> (E)     -12
    OP,        /// E -> i       -13
    NOT_A_RULE /// rule doesn't exist
} Prec_rules_enum;

/**
 * @enum Symbols used for precednece analysis.
 */
typedef enum
{
    EQ,        /// ==  0
    NEQ,       /// !=  1
    LEQ,       /// <=  2
    LTN,       /// <   3
    GEQ,       /// >=  4
    GTN,       /// >   5
    ADD,       /// +   6
    SUB,       /// -   7
    MUL,       /// *   8
    DIV,       /// /   9
    LPAR,      /// (   10
    RPAR,      /// )   11
    ID,        /// ID  12
    INT_NUM,   /// int 13
    FLOAT_NUM, /// float   14
    DOLLAR,    /// $   15
    NTERMINAL  /// noterminal
} Prec_table_symbol_enum;

typedef union stack_symbol_t
{
    Prec_table_symbol_enum precedence;
    char operand;
} Stack_symbol;

#endif //_EXPRESSION_H