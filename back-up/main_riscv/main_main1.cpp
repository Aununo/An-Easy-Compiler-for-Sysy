#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <koopa.h>

#include "../include/ast.hpp"
using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

void VisitProgram(const koopa_raw_program_t &program);
void VisitSlice(const koopa_raw_slice_t &slice);
void VisitFunction(const koopa_raw_function_t &func);
void VisitBasicBlock(const koopa_raw_basic_block_t &bb);
void VisitValue(const koopa_raw_value_t &value);
void VisitReturn(const koopa_raw_return_t &ret);
void VisitInteger(const koopa_raw_integer_t &integer);

int main(int argc, const char *argv[]) {
  assert(argc == 5);
  auto input = argv[2];
  auto output = argv[4];

  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  stringstream ir_stream;
  streambuf* old_buf = cout.rdbuf(ir_stream.rdbuf()); 

  // 生成 Koopa IR
  ast->GenIR(); 

  cout.rdbuf(old_buf);

  // 将 Koopa IR 转换为字符串
  string ir_code = ir_stream.str();
  const char* str = ir_code.c_str();  

  // 解析 Koopa IR
  koopa_program_t program;
  koopa_error_code_t ret_parse = koopa_parse_from_string(str, &program);
  assert(ret_parse == KOOPA_EC_SUCCESS);

  // 构建 raw program
  koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
  koopa_raw_program_t raw = koopa_build_raw_program(builder, program);

  // 释放 Koopa IR 占用的内存
  koopa_delete_program(program);

  // 处理 raw program，生成 RISC-V 汇编
  freopen(output, "w", stdout);
  VisitProgram(raw);

  // 处理完成，释放 raw program builder 占用的内存
  koopa_delete_raw_program_builder(builder);

  return 0;
}


void VisitProgram(const koopa_raw_program_t &program) {
  // 访问所有函数
  VisitSlice(program.funcs);
}

// 访问 raw slice
void VisitSlice(const koopa_raw_slice_t &slice) {
  for (size_t i = 0; i < slice.len; ++i) {
    auto ptr = slice.buffer[i];
    // 根据 slice 的 kind 决定将 ptr 视作何种元素
    switch (slice.kind) {
      case KOOPA_RSIK_FUNCTION:
        // 访问函数
        VisitFunction(reinterpret_cast<koopa_raw_function_t>(ptr));
        break;
      case KOOPA_RSIK_BASIC_BLOCK:
        // 访问基本块
        VisitBasicBlock(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
        break;
      case KOOPA_RSIK_VALUE:
        // 访问指令
        VisitValue(reinterpret_cast<koopa_raw_value_t>(ptr));
        break;
      default:
        assert(false);
    }
  }
}

// 访问函数
void VisitFunction(const koopa_raw_function_t &func) {
  // 输出 RISC-V 汇编的函数头部
  cout << ".text" << endl;

  // 去掉函数名中的 '@'
  string func_name = func->name;
  if (func_name[0] == '@') {
    func_name = func_name.substr(1);  // 移除第一个字符
  }

  cout << ".globl " << func_name << endl;
  cout << func_name << ":" << endl;

  // 访问所有基本块
  VisitSlice(func->bbs);
}


// 访问基本块
void VisitBasicBlock(const koopa_raw_basic_block_t &bb) {
  // 遍历基本块中的每条指令
  VisitSlice(bb->insts);
}

// 访问指令
void VisitValue(const koopa_raw_value_t &value) {
  const auto &kind = value->kind;
  switch (kind.tag) {
    case KOOPA_RVT_RETURN: {
      // 处理 return 指令
      VisitReturn(kind.data.ret);
      break;
    }
    case KOOPA_RVT_INTEGER: {
      // 处理 integer 常量
      VisitInteger(kind.data.integer);
      break;
    }
    default:
      assert(false); 
  }
}

// 处理 return 指令
void VisitReturn(const koopa_raw_return_t &ret) {
  // 获取 return 指令的返回值
  koopa_raw_value_t ret_value = ret.value;
  // 返回值是一个 integer
  if (ret_value->kind.tag == KOOPA_RVT_INTEGER) {
    int32_t int_val = ret_value->kind.data.integer.value;
    // 生成将立即数加载到 a0 寄存器的 RISC-V 汇编
    cout << "  li a0, " << int_val << endl;
  }
  // 生成 RISC-V 的 ret 指令
  cout << "  ret" << endl;
}

// 处理 integer 指令
void VisitInteger(const koopa_raw_integer_t &integer) {
}