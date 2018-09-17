#ifndef _ir1_instructions_h_
#define _ir1_instructions_h_

#include "ir_base.h"

// Intermediate representation 1
// IR1 doesn't support SSA form.
namespace IR1 {
  // Integer
  class Int : public IRBase {
    public:
      Int() {}
      virtual ~Int() {}
  };

  // String
  class Str : public IRBase {
    public:
      Str() {}
      virtual ~Str() {}
  };

  // Variable
  class Var : public IRBase {
    public:
      Var() {}
      virtual ~Var() {}
  };

  class Assign : public IRBase {
    private:
      IRBase* lhs;
      IRBase* rhs;

    public:
      Assign() {}
      virtual ~Assign() {}
  };

  class CondBranch: public IRBase {
    public:
      CondBranch() {}
      virtual ~CondBranch() {}
  };

  class Branch: public IRBase {
    public:
      Branch() {}
      virtual ~Branch() {}
  };

  class Switch: public IRBase {
    public:
      Switch() {}
      virtual ~Switch() {}
  };

  class LabelStmt : public IRBase {
    public:
      LabelStmt() {}
      virtual ~LabelStmt() {}
  };

  class ExprStmt : public IRBase {
    public:
      ExprStmt() {}
      virtual ~ExprStmt() {}
  };

  class Return: public IRBase {
    public:
      Return() {}
      virtual ~Return() {}
  };

  class UnaryOp: public IRBase {
    public:
      UnaryOp() {}
      virtual ~UnaryOp() {}
  };
  
  class BinOp: public IRBase {
    public:
      BinOp() {}
      virtual ~BinOp() {}
  };
  
  class Call: public IRBase {
    public:
      Call() {}
      virtual ~Call() {}
  };

  class Addr: public IRBase {
    public:
      Addr() {}
      virtual ~Addr() {}
  };

  class Mem: public IRBase {
    public:
      Mem() {}
      virtual ~Mem() {}
  };
};

#endif
