#ifndef _ir1_instructions_h_
#define _ir1_instructions_h_

#include "core/simple_vector.h"


namespace IR1 {
  class Value {
    protected:
      SimpleVector<Value*> use_list_;

    public:
      Value();
      ~Value();
  };

  class Instruction {
    public:
      Instruction() {}
      virtual ~Instruction() {}
  };

  class AssignInst : public Instruction {
    private:
      Instruction* lhs;
      Instruction* rhs;

    public:
      AssignInst() {}
      virtual ~AssignInst() {}
  };

  class BranchInst : public Instruction {
    Value* cond;
    public:
      BranchInst() {}
      virtual ~BranchInst() {}
  };

  class SwitchInst : public Instruction {
    public:
      SwitchInst() {}
      virtual ~SwitchInst() {}
  };

  class LabelInst : public Instruction {
    public:
      LabelInst() {}
      virtual ~LabelInst() {}
  };

  class ReturnInst : public Instruction {
    public:
      ReturnInst() {}
      virtual ~ReturnInst() {}
  };

  class UnaryInst : public Instruction {
    public:
      UnaryInst() {}
      virtual ~UnaryInst() {}
  };
  
  class BinaryInst : public Instruction {
    public:
      BinaryInst() {}
      virtual ~BinaryInst() {}
  };
  
  class CallInst : public Instruction {
    public:
      CallInst() {}
      virtual ~CallInst() {}
  };

  class AddrInst : public Instruction {
    public:
      AddrInst() {}
      virtual ~AddrInst() {}
  };

  class MemInst : public Instruction {
    public:
      MemInst() {}
      virtual ~MemInst() {}
  };

  class AllocaInst : public Instruction {
    public:
      AllocaInst() {}
      virtual ~AllocaInst() {}
  };

};

#endif
