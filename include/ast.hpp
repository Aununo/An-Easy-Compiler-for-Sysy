#pragma once
#include <iostream>
#include <memory>
#include <string>

// 全局临时变量计数器
static int temp_counter = 0;

// 获取新的临时变量名
inline std::string new_temp() {
    return "%" + std::to_string(temp_counter++);
}

class BaseAST {
  public:
   virtual ~BaseAST() = default;
   virtual void Dump() const = 0;
   virtual std::string GenIR() const = 0; // 返回生成的变量名
};

// 其他类定义保持不变...


class CompUnitAST : public BaseAST {
  public:
   std::unique_ptr<BaseAST> func_def;

   CompUnitAST(std::unique_ptr<BaseAST> func_def) : func_def(std::move(func_def)) {}

   void Dump() const override {
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
  }

  std::string GenIR() const override {
    std::cout << "fun @main(): i32 {\n";
    std::cout << "%entry:\n";
    func_def->GenIR();
    std::cout << "}\n";
    return "";  // 返回空，因为 CompUnitAST 不生成具体变量
  }
};


class FuncDefAST : public BaseAST {
  public:
   std::unique_ptr<BaseAST> func_type;
   std::string ident;
   std::unique_ptr<BaseAST> block;

   FuncDefAST(std::unique_ptr<BaseAST> func_type, std::string ident, std::unique_ptr<BaseAST> block)
      : func_type(std::move(func_type)), ident(std::move(ident)), block(std::move(block)) {}

   void Dump() const override {
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
  }

  std::string GenIR() const override {
    return block->GenIR(); // 继续生成 block 的 IR
  }
};


class FuncTypeAST : public BaseAST {
  public:
   std::string type;

   FuncTypeAST(std::string type) : type(std::move(type)) {}

   void Dump() const override {
    std::cout << "FuncTypeAST { " << type << " }";
  }

  std::string GenIR() const override {
    return "";  // FuncTypeAST 不生成IR
  }
};


class BlockAST : public BaseAST {
  public:
   std::unique_ptr<BaseAST> stmt;

   BlockAST(std::unique_ptr<BaseAST> stmt) : stmt(std::move(stmt)) {}

   void Dump() const override {
    std::cout << "BlockAST { ";
    stmt->Dump();
    std::cout << " }";
  }

  std::string GenIR() const override {
    return stmt->GenIR(); // 继续生成语句的 IR
  }
};

class StmtAST : public BaseAST {
  public:
   std::unique_ptr<BaseAST> number;

   StmtAST(std::unique_ptr<BaseAST> number) : number(std::move(number)) {}

   void Dump() const override {
    std::cout << "StmtAST { return ";
    number->Dump();
    std::cout << "; }";
  }

   std::string GenIR() const override {
    std::string result = number->GenIR();  // 获取表达式的最终结果
    std::cout << "  ret " << result << "\n";
    return result;  // 返回结果
  }
};


class NumberAST : public BaseAST {
public:
    int value;

    NumberAST(int value) : value(value) {}

    void Dump() const override {
        std::cout << "Number(" << value << ")";
    }

    std::string GenIR() const override {
    return std::to_string(value);  // 直接返回数字的字符串表示，不创建临时变量
}

};


class UnaryExpAST : public BaseAST {
public:
    char op;  // 记录运算符
    std::unique_ptr<BaseAST> operand;  // 运算对象

    UnaryExpAST(char op, std::unique_ptr<BaseAST> operand)
        : op(op), operand(std::move(operand)) {}

    void Dump() const override {
        // 输出运算表达式的结构
        std::cout << "UnaryExpAST(" << op << ", ";
        operand->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
    std::string operand_temp = operand->GenIR();  // 获取操作数的结果

    if (op == '-') {
        std::string result_temp = new_temp();  // 为当前运算生成新的临时变量
        std::cout << "  " << result_temp << " = sub 0, " << operand_temp << "\n";
        return result_temp;
    } else if (op == '!') {
        std::string result_temp = new_temp();  // 为当前运算生成新的临时变量
        std::cout << "  " << result_temp << " = eq " << operand_temp << ", 0\n";
        return result_temp;
    }

    // + 运算不产生实际IR，可以忽略
    return operand_temp;  // 对于 + 运算直接返回操作数的结果
}

};
