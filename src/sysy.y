%code requires {
  #define YYLTYPE_IS_DECLARED 1
  extern int yylineno;
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
  char op_val;
}

%token INT RETURN
%token <str_val> IDENT
%token <int_val> INT_CONST
%token AND_OP OR_OP EQ_OP NEQ_OP LE_OP GE_OP


%left OR_OP
%left AND_OP
%left EQ_OP NEQ_OP
%left LE_OP GE_OP
%left '+' '-'
%left '*' '/' '%'
%left '!' 

%type <ast_val> CompUnit FuncDef FuncType Block Stmt Exp LOrExp LAndExp EqExp RelExp AddExp MulExp UnaryExp PrimaryExp Number
%type <op_val> UnaryOp

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
    delete $2;
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
  : RETURN Exp ';' {
    $$ = new StmtAST(std::unique_ptr<BaseAST>($2)); 
  }
  ;

Exp
  : LOrExp { $$ = $1; }
  ;

LOrExp
  : LAndExp { $$ = $1; }
  | LOrExp OR_OP LAndExp { $$ = new LOrExpAST(std::unique_ptr<BaseAST>($1), std::unique_ptr<BaseAST>($3)); }
  ;

LAndExp
  : EqExp { $$ = $1; }
  | LAndExp AND_OP EqExp { $$ = new LAndExpAST(std::unique_ptr<BaseAST>($1), std::unique_ptr<BaseAST>($3)); }
  ;

EqExp
  : RelExp { $$ = $1; }
  | EqExp EQ_OP RelExp { $$ = new EqExpAST(std::unique_ptr<BaseAST>($1), "==", std::unique_ptr<BaseAST>($3)); }
  | EqExp NEQ_OP RelExp { $$ = new EqExpAST(std::unique_ptr<BaseAST>($1), "!=", std::unique_ptr<BaseAST>($3)); }
  ;

RelExp
  : AddExp { $$ = $1; }
  | RelExp '<' AddExp { $$ = new RelExpAST(std::unique_ptr<BaseAST>($1), "<", std::unique_ptr<BaseAST>($3)); }
  | RelExp '>' AddExp { $$ = new RelExpAST(std::unique_ptr<BaseAST>($1), ">", std::unique_ptr<BaseAST>($3)); }
  | RelExp LE_OP AddExp { $$ = new RelExpAST(std::unique_ptr<BaseAST>($1), "<=", std::unique_ptr<BaseAST>($3)); }
  | RelExp GE_OP AddExp { $$ = new RelExpAST(std::unique_ptr<BaseAST>($1), ">=", std::unique_ptr<BaseAST>($3)); }
  ;

AddExp
  : MulExp { $$ = $1; }
  | AddExp '+' MulExp { $$ = new AddExpAST(std::unique_ptr<BaseAST>($1), '+', std::unique_ptr<BaseAST>($3)); }
  | AddExp '-' MulExp { $$ = new AddExpAST(std::unique_ptr<BaseAST>($1), '-', std::unique_ptr<BaseAST>($3)); }
  ;

MulExp
  : UnaryExp { $$ = $1; }
  | MulExp '*' UnaryExp { $$ = new MulExpAST(std::unique_ptr<BaseAST>($1), '*', std::unique_ptr<BaseAST>($3)); }
  | MulExp '/' UnaryExp { $$ = new MulExpAST(std::unique_ptr<BaseAST>($1), '/', std::unique_ptr<BaseAST>($3)); }
  | MulExp '%' UnaryExp { $$ = new MulExpAST(std::unique_ptr<BaseAST>($1), '%', std::unique_ptr<BaseAST>($3)); }
  ;

UnaryExp
  : PrimaryExp
  | UnaryOp UnaryExp { $$ = new UnaryExpAST($1, std::unique_ptr<BaseAST>($2)); }
  ;

PrimaryExp
  : '(' Exp ')' { $$ = $2; }
  | Number { $$ = $1; }
  ;

UnaryOp
  : '+' { $$ = '+'; }
  | '-' { $$ = '-'; }
  | '!' { $$ = '!'; }
  ;

Number
  : INT_CONST { $$ = new NumberAST($1); }
  ;

%%

void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "Error: " << s << " at line " << yylineno << endl;
}