#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>

void VisitProgram(const koopa_raw_program_t &program);
void VisitSlice(const koopa_raw_slice_t &slice);
void VisitFunction(const koopa_raw_function_t &func);
void VisitBasicBlock(const koopa_raw_basic_block_t &bb);
void VisitValue(const koopa_raw_value_t &value);
void VisitReturn(const koopa_raw_return_t &ret);
void VisitInteger(const koopa_raw_integer_t &integer);

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