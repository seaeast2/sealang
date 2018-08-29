#ifndef _ir_instructions_h_
#define _ir_instructions_h_


namespace IR {
  // instructions base
  class Instruction {
    public:
      Instruction();
      virtual ~Instruction();
  };


  // AllocaInst
  // LoadInst
  // StoreInst
  // ICmpInst
  // (FCmpInst)
  // CallInst
  // ReturnInst
  // BranchInst
  // SwitchInst
  // BinOp
  // MemInst
  // CastInst
};

#endif
