#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include "../include/ast.hpp"
using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

int main(int argc, const char *argv[]) {

  assert(argc == 5);
  //auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  stringstream ir_stream;
  streambuf* old_buf = cout.rdbuf(ir_stream.rdbuf());
  //ast->Dump();
  //cout << endl;
  string ir_code = ir_stream.str();
  const char* str = ir_code.c_str();
  
  freopen(output, "w", stdout);

  ast->GenIR();
  //cout << endl;

  fclose(stdout);
  return 0;
}
