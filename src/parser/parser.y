%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern void yyerror(const char* s);

AST* parse_tree;
%}

%union {
    int ival;
    float fval;
    char* sval;
    AST* ast;
    std::vector<AST*>* ast_list;
}

%token <ival> INTEGER
%token <fval> FLOAT
%token <sval> STRING IDENTIFIER
%token SELECT FROM WHERE INSERT INTO VALUES UPDATE DELETE
%token CREATE TABLE DROP ALTER INDEX ON
%token AND OR NOT NULL_TOKEN
%token INTEGER_TYPE VARCHAR_TYPE BOOLEAN_TYPE FLOAT_TYPE
%token EQ NE LT LE GT GE
%token LPAREN RPAREN COMMA SEMICOLON STAR UNKNOWN

%type <ast> statement select_stmt insert_stmt create_stmt
%type <ast> where_clause expression condition
%type <ast_list> column_list value_list table_list

%left OR
%left AND
%right NOT

%%

program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    select_stmt SEMICOLON { parse_tree = $1; }
    | insert_stmt SEMICOLON { parse_tree = $1; }
    | create_stmt SEMICOLON { parse_tree = $1; }
    ;

select_stmt:
    SELECT column_list FROM table_list where_clause {
        $$ = new SelectAST($2, $4, $5);
    }
    ;

insert_stmt:
    INSERT INTO IDENTIFIER LPAREN column_list RPAREN VALUES LPAREN value_list RPAREN {
        $$ = new InsertAST($3, $5, $9);
    }
    ;

create_stmt:
    CREATE TABLE IDENTIFIER LPAREN column_def_list RPAREN {
        $$ = new CreateTableAST($3, $5);
    }
    ;

where_clause:
    /* empty */ { $$ = nullptr; }
    | WHERE expression { $$ = $2; }
    ;

expression:
    condition
    | expression AND expression { $$ = new BinaryOpAST(OpType::AND, $1, $3); }
    | expression OR expression { $$ = new BinaryOpAST(OpType::OR, $1, $3); }
    | NOT expression { $$ = new UnaryOpAST(OpType::NOT, $2); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%