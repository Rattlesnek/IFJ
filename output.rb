.IFJcode18
=>Check_err: INT
=>Check_err: ==
=>Check_err: INT
DEFVAR GF@INT0INT
EQ GF@INT0INT int@1 int@1
=>Check_err: BOOL_ID
tmp->name: BOOL_ID

JUMPIFEQ $else$0 GF@INT0INT bool@false

WRITE int@1

JUMP $endif$0
LABEL $else$0

