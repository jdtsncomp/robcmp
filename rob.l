%{
#include "node.h"
#include "bison.hpp"
%}

%option yylineno
%x COMMENT

%%

[ \t\n\r]				{ /* espaços */ }
"/*"					{ yymore(); BEGIN COMMENT; }
<COMMENT>[^*]			{ yymore(); }
<COMMENT>"*"/[^/]		{ yymore(); }
<COMMENT>"*/"			{ BEGIN(INITIAL); }

"//".*$					{ fprintf(stderr, "Comentario: %s\n", yytext); /* comentario de linha */}

"magnet"				{ return(TOK_MAGNET); }
"rotate"				{ return(TOK_ROTATE); }
"wait_button_press"		{ return(TOK_WAIT_BUTTON_PRESS); }

-?[0-9]+				{ yylval.nint = atoi(yytext);
						  return(TOK_INTEIRO); }

";"						{ return(';'); }

%%
