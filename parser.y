%{
#include <stdlib.h>
#include "machine.h"

%}

%union {
  int i;
  char *strval;
  struct Operand *oper;
  struct Instruction *inst;
}
%token TOKNUM
%token TOKREG
%token TOKCOMA
%token TOKOPAREN
%token TOKCR
%token TOKCPAREN
%token TOKNUMBER 
%token TOKID
%token TOKIMM
%type <i> TOKIMM TOKNUMBER 
%type <strval> TOKREG TOKID TOKLABEL 
%type <oper> operand
%type <inst> inst
%token TOKNOP
%token TOKMOV
%token TOKSW
%token TOKLW
%token TOKPUSH
%token TOKPOP
%token TOKPRINT
%token TOKREAD
%token TOKADD
%token TOKSUB
%token TOKDIV
%token TOKMUL
%token TOKCMP
%token TOKJMP
%token TOKJMPE
%token TOKJMPL
%token TOKHLT
%token TOKLABEL
%token TOKAND
%token TOKOR
%token TOKXOR
%token TOKLSH
%token TOKRSH
%token TOKCALL
%token TOKRET
%start input


%%

input       : /* empty */
            | input inst TOKCR { code[count++]=*$2;}
;

inst: 
      TOKNOP { $$ = malloc(sizeof(struct Instruction)); $$->op=NOP;}
    | TOKMOV operand TOKCOMA operand  { $$ = malloc(sizeof(struct Instruction)); $$->op=MOV; $$->src=*$2; $$->dst=*$4;}
  
    | TOKSW operand TOKCOMA operand  { $$ = malloc(sizeof(struct Instruction)); $$->op=SW; $$->src=*$2; $$->dst=*$4;}
    | TOKLW operand TOKCOMA operand  { $$ = malloc(sizeof(struct Instruction)); $$->op=LW; $$->src=*$2; $$->dst=*$4;}
    | TOKPUSH operand { $$ = malloc(sizeof(struct Instruction)); $$->op=PUSH; $$->src=*$2;}
    | TOKPOP operand  { $$ = malloc(sizeof(struct Instruction)); $$->op=POP; $$->src=*$2;}
    | TOKPRINT operand { $$ = malloc(sizeof(struct Instruction)); $$->op=PRINT ; $$->src=*$2;}
    | TOKREAD operand  { $$ = malloc(sizeof(struct Instruction)); $$->op=READ; $$->src=*$2;}
    | TOKADD operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=ADD; $$->src=*$2; $$->dst=*$4;}
    | TOKSUB operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=SUB; $$->src=*$2; $$->dst=*$4;}
    | TOKMUL operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=MUL; $$->src=*$2; $$->dst=*$4;}
    | TOKDIV operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=DIV; $$->src=*$2; $$->dst=*$4;}
    | TOKCMP operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=CMP; $$->src=*$2; $$->dst=*$4;}
    | TOKJMP operand { $$ = malloc(sizeof(struct Instruction)); $$->op=JMP; $$->src=*$2; }
    | TOKJMPE operand { $$ = malloc(sizeof(struct Instruction)); $$->op=JMPE; $$->src=*$2; }
    | TOKJMPL operand { $$ = malloc(sizeof(struct Instruction)); $$->op=JMPL; $$->src=*$2; }
    | TOKLABEL { $$ = malloc(sizeof(struct Instruction)); $$->op=LABEL; $$->src.lab=$1; }
    | TOKHLT { $$ = malloc(sizeof(struct Instruction)); $$->op=HLT;}
    | TOKAND operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=AND; $$->src=*$2; $$->dst=*$4;}
    | TOKOR operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=OR; $$->src=*$2; $$->dst=*$4;}
    | TOKXOR operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=XOR; $$->src=*$2; $$->dst=*$4;}
    | TOKLSH operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=LSH; $$->src=*$2; $$->dst=*$4;}
    | TOKRSH operand TOKCOMA operand { $$ = malloc(sizeof(struct Instruction)); $$->op=RSH; $$->src=*$2; $$->dst=*$4;}
	| TOKCALL operand { $$ = malloc(sizeof(struct Instruction)); $$->op=CALL; $$->src=*$2; }
	| TOKRET { $$ = malloc(sizeof(struct Instruction)); $$->op=RET;}
    ;


operand:
          TOKREG { $$ = malloc(sizeof(struct Operand)); $$->type=REG; $$->val=reg($1);}
        | TOKOPAREN TOKREG TOKCPAREN { $$ = malloc(sizeof(struct Operand)); $$->type=MEM; $$->val=reg($2);}
        | TOKNUMBER { $$ = malloc(sizeof(struct Operand)); $$->type=MEM; $$->val=$1;}
        | TOKIMM { $$ = malloc(sizeof(struct Operand)); $$->type=IMM; $$->val=$1;}
        | TOKID  { $$ = malloc(sizeof(struct Operand)); $$->type=LABELOP; $$->lab=$1;}
        ;

%%



