.IFJcode24
DEFVAR GF@result
DEFVAR GF@a
MOVE GF@a int@12
DEFVAR GF@b
MOVE GF@b int@20
LABEL $main
CREATEFRAME
PUSHFRAME
CONST LF@c
MOVE LF@c string@+
DEFVAR LF@temp_var_e0
GT LF@temp_var_e0 LF@c LF@0
LABEL $if_start_0
JUMPIFEQ$if_0 LF@temp_var_e0
WRITE Vysledek: 
WRITE c
LABEL $if_end_0
POPFRAME
RETURN