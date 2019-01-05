#ifndef _ir1_instructions_h_
#define _ir1_instructions_h_

#include "ir_base.h"

// Intermediate representation 1
// IR1 doesn't support SSA form.
namespace IR {
  // Integer
  class Int : public IR {
    public:
      Int() {}
      virtual ~Int() {}
  };

  // String
  class Str : public IR {
    public:
      Str() {}
      virtual ~Str() {}
  };

  // Variable
  class Var : public IR {
    public:
      Var() {}
      virtual ~Var() {}
  };

  class Assign : public IR {
    private:
      IR* lhs;
      IR* rhs;

    public:
      Assign() {}
      virtual ~Assign() {}
  };

  class CondBranch: public IR {
    public:
      CondBranch() {}
      virtual ~CondBranch() {}
  };

  class Branch: public IR {
    public:
      Branch() {}
      virtual ~Branch() {}
  };

  class Switch: public IR {
    public:
      Switch() {}
      virtual ~Switch() {}
  };

  class LabelStmt : public IR {
    public:
      LabelStmt() {}
      virtual ~LabelStmt() {}
  };

  class ExprStmt : public IR {
    public:
      ExprStmt() {}
      virtual ~ExprStmt() {}
  };

  class Return: public IR {
    public:
      Return() {}
      virtual ~Return() {}
  };

  class UnaryOp: public IR {
    public:
      UnaryOp() {}
      virtual ~UnaryOp() {}
  };
  
  class BinOp: public IR {
    public:
      BinOp() {}
      virtual ~BinOp() {}
  };
  
  class Call: public IR {
    public:
      Call() {}
      virtual ~Call() {}
  };

  class Addr: public IR {
    public:
      Addr() {}
      virtual ~Addr() {}
  };

  class Mem: public IR {
    public:
      Mem() {}
      virtual ~Mem() {}
  };
};

#endif
