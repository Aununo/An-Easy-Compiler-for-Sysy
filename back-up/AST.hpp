#pragma once
#include <iostream>
#include <memory>
#include <string>

class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual void Dump() const = 0;
};

class CompUnitAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;

  CompUnitAST(std::unique_ptr<BaseAST> func_def)
      : func_def(std::move(func_def)) {}

  void Dump() const override {
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
  }
};

class FuncDefAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

  FuncDefAST(std::unique_ptr<BaseAST> func_type, const std::string &ident, std::unique_ptr<BaseAST> block)
      : func_type(std::move(func_type)), ident(ident), block(std::move(block)) {}

  void Dump() const override {
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
  }
};

class FuncTypeAST : public BaseAST {
 public:
  std::string type;

  FuncTypeAST(const std::string &type) : type(type) {}

  void Dump() const override {
    std::cout << "FuncTypeAST { " << type << " }";
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
};

class NumberAST : public BaseAST {
 public:
  int value;

  NumberAST(int value) : value(value) {}

  void Dump() const override {
    std::cout << "NumberAST { " << value << " }";
  }
};

