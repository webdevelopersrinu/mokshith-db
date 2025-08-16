%{
#include "parser.h"
#include <string>
%}

%option noyywrap
%option case-insensitive

%%

[ \t\n]+        { /* Skip whitespace */ }
"--".*          { /* Skip comments */ }

SELECT          { return SELECT; }
FROM            { return FROM; }
WHERE           { return WHERE; }
INSERT          { return INSERT; }
INTO            { return INTO; }
VALUES          { return VALUES; }
UPDATE          { return UPDATE; }
DELETE          { return DELETE; }
CREATE          { return CREATE; }
TABLE           { return TABLE; }
DROP            { return DROP; }
ALTER           { return ALTER; }
INDEX           { return INDEX; }
ON              { return ON; }

AND             { return AND; }
OR              { return OR; }
NOT             { return NOT; }
NULL            { return NULL_TOKEN; }

INT|INTEGER     { return INTEGER_TYPE; }
VARCHAR         { return VARCHAR_TYPE; }
BOOLEAN|BOOL    { return BOOLEAN_TYPE; }
FLOAT|DOUBLE    { return FLOAT_TYPE; }

"="             { return EQ; }
"<>"            { return NE; }
"<"             { return LT; }
"<="            { return LE; }
">"             { return GT; }
">="            { return GE; }

"("             { return LPAREN; }
")"             { return RPAREN; }
","             { return COMMA; }
";"             { return SEMICOLON; }
"*"             { return STAR; }

[0-9]+          { yylval.ival = atoi(yytext); return INTEGER; }
[0-9]+\.[0-9]+  { yylval.fval = atof(yytext); return FLOAT; }
'[^']*'         { yylval.sval = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }

.               { return UNKNOWN; }

%%