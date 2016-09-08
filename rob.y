%{
#include "node.h"
#include <stdio.h>
%}

%token TOK_MAGNET TOK_ROTATE TOK_WAIT_BUTTON_PRESS TOK_INTEIRO

%union {
	int nint;
	Node *node;
	Stmts *stmt;
}

%type <node> number stmt
%type <stmt> stmts
%type <nint> TOK_INTEIRO

%start programa

%%

programa : stmts    					{ Program p; p.generate($1); }
		 ;

stmts : stmts stmt						{ $$->append($2); }
	  | stmt							{ $$ = new Stmts($1); }
	  ;

stmt : TOK_MAGNET number';'				{ $$ = MagnetCommand($2); }	 
	 | TOK_ROTATE number number';'		{ $$ = RotateCommand($2, $3); }
	 | TOK_WAIT_BUTTON_PRESS';'			{ $$ = WaitButtonPressCommand(); }
	 ;

number : TOK_INTEIRO					{ $$ = new Int16($1); }
	   ;

%%

void yyerror(const char *s){
	fprintf(stderr, "%s:%d: error: %s %s\n", build_filename, yylineno, s, yytext);
	exit(1);
}

extern "C" int yywrap() {
	return 1;
}
