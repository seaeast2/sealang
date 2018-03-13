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
  class VoidType : public Type {
    public:
      VoidType() {}
      virtual ~VoidType() {}
  };

  class BoolType : public Type {
    public:
      BoolType() {}
      virtual ~BoolType() {}
  };

  class IntegerType : public Type {
    public:
      IntegerType() {}
      virtual ~IntegerType() {}
  };

  class RealType : public Type {
    public:
      RealType() {}
      virtual ~RealType() {}
  };

  class NamedType : public Type {
    public:
      NamedType() {}
      virtual ~NamedType() {}
  };

  class CompositeType : public Type {
    public:
      CompositeType() {}
      virtual ~CompositeType() {}
  };

  class ClassType : public CompositeType {
    public:
      ClassType() {}
      virtual ~ClassType() {}
  };

  class PointerType : public Type {
    public:
      PointerType() {}
      virtual ~PointerType() {}
  };

}
#endif
