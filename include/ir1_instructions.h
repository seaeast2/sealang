#ifndef _ir1_instructions_h_
#define _ir1_instructions_h_

#include "core/simple_vector.h"

// Intermediate representation 1
// IR1 doesn't support SSA form.
namespace IR1 {
  class IR1Base {
    public:
      IR1Base() {}
      virtual ~IR1Base() {}
  };

  // Integer
  class Int : public IR1Base {
    public:
      Int() {}
      virtual ~Int() {}
  };

  // String
  class Str: public IR1Base {
    public:
      Str() {}
      virtual ~Str() {}
  };

  // Variable
  class Var : public IR1Base {
    public:
      Var() {}
      virtual ~Var() {}
  };

  class Assign: public IR1Base {
    private:
      IR1Base* lhs;
      IR1Base* rhs;

    public:
      Assign() {}
      virtual ~Assign() {}
  };

  class Branch: public IR1Base {
    public:
      Branch() {}
      virtual ~Branch() {}
  };

  class Switch: public IR1Base {
    public:
      Switch() {}
      virtual ~Switch() {}
  };

  class LabelStmt : public IR1Base {
    public:
      LabelStmt() {}
      virtual ~LabelStmt() {}
  };

  class ExprStmt : public IR1Base {
    public:
      ExprStmt() {}
      virtual ~ExprStmt() {}
  };

  class Return: public IR1Base {
    public:
      Return() {}
      virtual ~Return() {}
  };

  class UnaryOp: public IR1Base {
    public:
      UnaryOp() {}
      virtual ~UnaryOp() {}
  };
  
  class BinOp: public IR1Base {
    public:
      BinOp() {}
      virtual ~BinOp() {}
  };
  
  class Call: public IR1Base {
    public:
      Call() {}
      virtual ~Call() {}
  };

  class Addr: public IR1Base {
    public:
      Addr() {}
      virtual ~Addr() {}
  };

  class Mem: public IR1Base {
    public:
      Mem() {}
      virtual ~Mem() {}
  };

};

#endif
