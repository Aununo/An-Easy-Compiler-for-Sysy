#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#define MOD

#include "../include/koopa.h"
#include "../include/ast.hpp"
#include "../include/riscv.hpp"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);


int main(int argc, const char *argv[]) {

  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  yyin = fopen(input, "r");
  #ifdef MOD
  freopen(output, "w", stdout); //stdout -> output
  #endif
  assert(yyin);

  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  if (string(mode) == "-koopa") ast->GenIR();

  else if (string(mode) == "-riscv") {
    stringstream ss;
    streambuf* cout_buf = cout.rdbuf();
    cout.rdbuf(ss.rdbuf());

    ast->GenIR();
    string ir_str = ss.str();
    const char *ir = ir_str.data();
    cout.rdbuf(cout_buf);

    koopa_program_t program;
    koopa_error_code_t ret = koopa_parse_from_string(ir, &program);
    assert(ret == KOOPA_EC_SUCCESS);

    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    koopa_delete_program(program);

    freopen(output, "w", stdout);
    VisitProgram(raw);
    koopa_delete_raw_program_builder(builder);
  }

  else if (string(mode) == "-tree") ast->Dump();
  else cout << "I have no idea" << endl;
  cout << endl;
  return 0;
}