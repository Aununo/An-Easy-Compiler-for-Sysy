%option noyywrap
%option nounput
%option noinput
%option yylineno


%{
#include <cstdlib>
#include <string>
#include "sysy.tab.hpp" 
using namespace std;

%}

WhiteSpace    [ \t\n\r]+
LineComment   "//".*
BlockComment  "/*"([^*]|\*[^/])*"*/"

Identifier    [a-zA-Z_][a-zA-Z0-9_]*
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

%%

{WhiteSpace}    { /* 忽略空白字符 */ }
{LineComment}   { /* 忽略单行注释 */ }
{BlockComment}  { /* 忽略块注释 */ }

"int"           { return INT; }
"return"        { return RETURN; }

{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 10); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 8); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 16); return INT_CONST; }

"&&"            { return AND_OP; }
"||"            { return OR_OP; }
"=="            { return EQ_OP; }
"!="            { return NEQ_OP; }
"<="            { return LE_OP; }
">="            { return GE_OP; }

.               { return yytext[0]; }

%%

