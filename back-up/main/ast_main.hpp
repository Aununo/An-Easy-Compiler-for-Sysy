#pragma once
#include <iostream>
#include <memory>
#include <string>

/* 
CompUnitAST { FuncDefAST { FuncTypeAST { int }, main, BlockAST { StmtAST { 0 } } } }

fun @main(): i32 { 
%entry:            
  ret 0            
}
*/

class BaseAST {
  public:
   virtual ~BaseAST() = default;
   virtual void Dump() const = 0;
   virtual void GenIR() const = 0;
};


class CompUnitAST : public BaseAST {
  public:
   std::unique_ptr<BaseAST> func_def;

   CompUnitAST(std::unique_ptr<BaseAST> func_def) : func_def(std::move(func_def)) {}

   void Dump() const override {
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
  }

  void GenIR() const override {
    std::cout << "fun @main(): i32 {\n";
    func_def->GenIR();
    std::cout << "}\n";
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

  void GenIR() const override {
    std::cout << "%entry:\n";  
    block->GenIR();
  }
};


class FuncTypeAST : public BaseAST {
  public:
   std::string type;


   FuncTypeAST(std::string type) : type(std::move(type)) {}

   void Dump() const override {
    std::cout << "FuncTypeAST { " << type << " }";
  }
  void GenIR() const override {
    
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

  void GenIR() const override {
    stmt->GenIR();
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

   void GenIR() const override {
    std::cout << "  ret "; 
    number->GenIR();
    std::cout << "\n";
  }
};


class NumberAST : public BaseAST {
  public:
   int value;


   NumberAST(int value) : value(value) {}

   void Dump() const override {
    std::cout << "NumberAST { " << value << " }";
  }

  void GenIR() const override {
    std::cout << value; 
  }
};
