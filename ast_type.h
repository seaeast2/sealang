#ifndef _ast_type_h_
#define _ast_type_h_

namespace AST {
  // These types are all AST Types.
  // Base type in AST
  class Type {
    public:
      Type() {};
      virtual ~Type() {};

      virtual void Print();
  };

  // Primitive types =================================================
  // void type
  class VoidType : public Type {
    public:
      VoidType() {}
      virtual ~VoidType() {}
  };

  // bool type
  class BoolType : public Type {
    public:
      BoolType() {}
      virtual ~BoolType() {}
  };

  // Integer base type
  class IntegerType : public Type {
    public:
      IntegerType() {}
      virtual ~IntegerType() {}
  };

  // Real type base
  class RealType : public Type {
    public:
      RealType() {}
      virtual ~RealType() {}
  };

  // Aggregation type 
  class AggregationType : public Type {
    public:
      AggregationType() {}
      virtual ~AggregationType() {}
  };

  // Class type
  class ClassType : public AggregationType {
    public:
      ClassType() {}
      virtual ~ClassType() {}
  };

  // Pointer type
  class PointerType : public Type {
    public:
      PointerType() {}
      virtual ~PointerType() {}
  };

}
#endif
