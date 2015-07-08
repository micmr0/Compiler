%{
#include <stdlib.h>
#include <string.h>
#include "def.tab.hh"
#define INFILE_ERROR 1
#define OUTFILE_ERROR 2
extern int yylineno;
int yyerror(const char *,...);
void writeLexValue(char *);
%}
%%
\+			{writeLexValue("+"); return('+');} 
\*			{writeLexValue("*"); return('*');}
\(			{writeLexValue("("); return('(');}
\)			{writeLexValue(")"); return(')');}
\[			{writeLexValue("["); return('[');}
\]			{writeLexValue("]"); return(']');}
\{			{writeLexValue("{"); return('{');}
\}			{writeLexValue("}"); return('}');}
\/			{writeLexValue("/"); return('/');}
\-			{writeLexValue("-"); return('-');}
\^			{writeLexValue("^"); return('^');}
\=			{
				writeLexValue("^\n"); return '=';
				writeLexValue("^\n");
				return '=';
			}
\:			{writeLexValue(":"); return(':');}
\"			{writeLexValue(":"); return('"');}
\;			{writeLexValue(";"); return(';');}
\==			{writeLexValue(";"); return EQ;}
\!=			{writeLexValue(";"); return NEQ;}
\>			{writeLexValue(">"); return('>');}
\<			{writeLexValue("<"); return('<');}
\>=			{writeLexValue(">=");return GE;}
\<=			{writeLexValue("<=");return LE;}
\read		{writeLexValue("read"); return READ;}
\print		{writeLexValue("print"); return PRINT;}
\printl		{writeLexValue("print"); return PRINTL;}
\if			{writeLexValue("if"); return IF;}
\else		{writeLexValue("if"); return ELSE;}
\and        {writeLexValue("and"); return AND;}
\while		{writeLexValue("while"); return WHILE;}
\int		{{writeLexValue("int"); return INT;}}
float		{{writeLexValue("float"); return FLOAT;}}
\string		{{writeLexValue("int"); return STRING;}}
\write		{writeLexValue("write"); yylval.text = strdup(yytext); return WRITE;}
[A-Za-z_][A-Za-z0-9_]*.\[[0-9]+\]+	{writeLexValue("tablica jednowymiarowa");}
0|[-][1-9][0-9]*		{writeLexValue("liczba calkowita ujemna");yylval.text = strdup(yytext); return LC;}
0|[1-9][0-9]*		{writeLexValue("liczba naturalna");yylval.ival = atoi(yytext); return LN;}
([-][0-9]|[1-9][0-9]*)\.+[0-9]+		{writeLexValue("liczba zmiennoprzecinkowa ujemna");yylval.fval = atof(yytext); return LFN;}	
([0-9]|[1-9][0-9]*)\.+[0-9]+		{writeLexValue("liczba zmiennoprzecinkowa");yylval.fval = atof(yytext); return LF;}	
[A-Za-z_][A-Za-z0-9_]*	{writeLexValue("nazwa"); yylval.text = strdup(yytext); return ID;}
[@][A-Za-z_][A-Za-z0-9_]*	{writeLexValue("nazwa tablicy"); yylval.text = strdup(yytext); return NAZWA_TAB;}
["][A-Za-z_][A-Za-z0-9_ \+\:\-\*\_\$\^\\n]*["]	{writeLexValue("string"); yylval.text = strdup(yytext); return STR;}
[1-9]+[.]{2}		{writeLexValue("dziwne");}
\ |\t			{;}
\n|#.*\n			{yylineno++;}
.			{yyerror("Błąd leksykalny\n");}
%%
int yyerror(const char *msg,...)
{
	printf("%d: %s", yylineno, msg);
	exit(1);
}

void writeLexValue(char *value)
{
	fprintf(yyout, "%d::%s\n", yylineno, value);
}


