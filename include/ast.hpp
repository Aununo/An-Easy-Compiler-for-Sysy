#pragma once
#include <iostream>
#include <memory>
#include <string>

static int temp_counter = 0;

inline std::string new_temp() {
    return "%" + std::to_string(temp_counter++);
}

class BaseAST {
  public:
   virtual ~BaseAST() = default;
   virtual void Dump() const = 0;
   virtual std::string GenIR() const = 0;
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

  std::string GenIR() const override {
    std::cout << "fun @main(): i32 {\n";
    std::cout << "%entry:\n";
    func_def->GenIR();
    std::cout << "}\n";
    return "";
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
    return block->GenIR();
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
    return "";
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
    return stmt->GenIR();
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
    std::string result = number->GenIR();
    std::cout << "  ret " << result << "\n";
    return result;
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
    return std::to_string(value);
}

};

class UnaryExpAST : public BaseAST {
public:
    char op; 
    std::unique_ptr<BaseAST> operand; 

    UnaryExpAST(char op, std::unique_ptr<BaseAST> operand)
        : op(op), operand(std::move(operand)) {}

    void Dump() const override {
        std::cout << "UnaryExpAST(" << op << ", ";
        operand->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
    std::string operand_temp = operand->GenIR();

    if (op == '-') {
        std::string result_temp = new_temp(); 
        std::cout << "  " << result_temp << " = sub 0, " << operand_temp << "\n";
        return result_temp;
    } else if (op == '!') {
        std::string result_temp = new_temp(); 
        std::cout << "  " << result_temp << " = eq " << operand_temp << ", 0\n";
        return result_temp;
    }

    return operand_temp; 
}
};

// 新增的用于关系表达式的类
class RelExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    std::string op;
    std::unique_ptr<BaseAST> rhs;

    RelExpAST(std::unique_ptr<BaseAST> lhs, std::string op, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), op(std::move(op)), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "RelExpAST(";
        lhs->Dump();
        std::cout << " " << op << " ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
        std::string lhs_temp = lhs->GenIR();
        std::string rhs_temp = rhs->GenIR();
        std::string result_temp = new_temp();

        if (op == "<") {
            std::cout << "  " << result_temp << " = lt " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == "<=") {
            std::cout << "  " << result_temp << " = le " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == ">") {
            std::cout << "  " << result_temp << " = gt " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == ">=") {
            std::cout << "  " << result_temp << " = ge " << lhs_temp << ", " << rhs_temp << "\n";
        }

        return result_temp;
    }
};

// 新增的用于相等表达式的类
class EqExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    std::string op;
    std::unique_ptr<BaseAST> rhs;

    EqExpAST(std::unique_ptr<BaseAST> lhs, std::string op, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), op(std::move(op)), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "EqExpAST(";
        lhs->Dump();
        std::cout << " " << op << " ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
        std::string lhs_temp = lhs->GenIR();
        std::string rhs_temp = rhs->GenIR();
        std::string result_temp = new_temp();

        if (op == "==") {
            std::cout << "  " << result_temp << " = eq " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == "!=") {
            std::cout << "  " << result_temp << " = ne " << lhs_temp << ", " << rhs_temp << "\n";
        }

        return result_temp;
    }
};

//
class LOrExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    std::unique_ptr<BaseAST> rhs;

    LOrExpAST(std::unique_ptr<BaseAST> lhs, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "LOrExpAST(";
        lhs->Dump();
        std::cout << " || ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
    std::string lhs_temp = lhs->GenIR();
    std::string rhs_temp = rhs->GenIR();
    std::string lhs_cmp = new_temp();
    std::string rhs_cmp = new_temp();
    std::string result_temp = new_temp();  // 用来存储最终结果

    // 比较 lhs 是否不为 0
    std::cout << "  " << lhs_cmp << " = ne " << lhs_temp << ", 0\n";

    // 比较 rhs 是否不为 0
    std::cout << "  " << rhs_cmp << " = ne " << rhs_temp << ", 0\n";

    // 使用按位或来模拟逻辑或
    std::cout << "  " << result_temp << " = or " << lhs_cmp << ", " << rhs_cmp << "\n";

    return result_temp;
}

};



class LAndExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    std::unique_ptr<BaseAST> rhs;

    LAndExpAST(std::unique_ptr<BaseAST> lhs, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "LAndExpAST(";
        lhs->Dump();
        std::cout << " && ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
    std::string lhs_temp = lhs->GenIR();
    std::string rhs_temp = rhs->GenIR();
    std::string lhs_cmp = new_temp();
    std::string rhs_cmp = new_temp();
    std::string result_temp = new_temp();  // 用来存储最终结果

    // 比较 lhs 是否不为 0
    std::cout << "  " << lhs_cmp << " = ne " << lhs_temp << ", 0\n";

    // 比较 rhs 是否不为 0
    std::cout << "  " << rhs_cmp << " = ne " << rhs_temp << ", 0\n";

    // 使用按位与来模拟逻辑与
    std::cout << "  " << result_temp << " = and " << lhs_cmp << ", " << rhs_cmp << "\n";

    return result_temp;
}

};



class AddExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    char op;
    std::unique_ptr<BaseAST> rhs;

    AddExpAST(std::unique_ptr<BaseAST> lhs, char op, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "AddExpAST(";
        lhs->Dump();
        std::cout << " " << op << " ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
        std::string lhs_temp = lhs->GenIR();
        std::string rhs_temp = rhs->GenIR();
        std::string result_temp = new_temp();

        if (op == '+') {
            std::cout << "  " << result_temp << " = add " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == '-') {
            std::cout << "  " << result_temp << " = sub " << lhs_temp << ", " << rhs_temp << "\n";
        }

        return result_temp;
    }
};

class MulExpAST : public BaseAST {
public:
    std::unique_ptr<BaseAST> lhs;
    char op;
    std::unique_ptr<BaseAST> rhs;

    MulExpAST(std::unique_ptr<BaseAST> lhs, char op, std::unique_ptr<BaseAST> rhs)
        : lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {}

    void Dump() const override {
        std::cout << "MulExpAST(";
        lhs->Dump();
        std::cout << " " << op << " ";
        rhs->Dump();
        std::cout << ")";
    }

    std::string GenIR() const override {
        std::string lhs_temp = lhs->GenIR();
        std::string rhs_temp = rhs->GenIR();
        std::string result_temp = new_temp();

        if (op == '*') {
            std::cout << "  " << result_temp << " = mul " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == '/') {
            std::cout << "  " << result_temp << " = div " << lhs_temp << ", " << rhs_temp << "\n";
        } else if (op == '%') {
            std::cout << "  " << result_temp << " = mod " << lhs_temp << ", " << rhs_temp << "\n";
        }

        return result_temp;
    }
};
