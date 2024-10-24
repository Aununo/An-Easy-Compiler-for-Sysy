%code requires {
  #include <memory>
  #include <string>
  #include "../include/ast.hpp"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "../include/ast.hpp"

int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}

%parse-param { std::unique_ptr<BaseAST> &ast }

%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

%token INT RETURN
%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt Number

%%

CompUnit
  : FuncDef {
    auto comp_unit = std::make_unique<CompUnitAST>(std::unique_ptr<BaseAST>($1));
    ast = std::move(comp_unit);
  }
  ;

FuncDef
  : FuncType IDENT '(' ')' Block {
    auto func_def = std::make_unique<FuncDefAST>(
        std::unique_ptr<BaseAST>($1),               
        *$2,                                       
        std::unique_ptr<BaseAST>($5));            
    $$ = func_def.release(); 
  }
  ;

FuncType
  : INT {
    $$ = new FuncTypeAST("int");
  }
  ;

Block
  : '{' Stmt '}' {
    auto block = std::make_unique<BlockAST>(std::unique_ptr<BaseAST>($2));
    $$ = block.release();
  }
  ;

Stmt
  : RETURN Number ';' {
    auto stmt = std::make_unique<StmtAST>(std::unique_ptr<BaseAST>($2));
    $$ = stmt.release();
  }
  ;

Number
  : INT_CONST {
    $$ = new NumberAST($1);
  }
  ;

%%

void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}