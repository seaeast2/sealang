#ifndef _ast_type_h_
#define _ast_type_h_

namespace AST {
  // These types are all AST Types.
  // Base type in AST
  class Type {
    protected:
      AstType() {};
    public:
      virtual ~AstType() {};

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

  // Char type
  class CharType : public IntegerType {
    public:
      CharType() {}
      virtual ~CharType() {}
  };

  // Short type
  class ShortType : public IntegerType {
    public:
      ShortType() {}
      virtual ~ShortType() {}
  };

  // Int type
  class IntType : public IntegerType {
    public:
      IntType() {}
      virtual ~IntType() {}
  };

  // Long type
  class LongType : public IntegerType {
    public:
      LongType() {}
      virtual ~LongType() {}
  };

  // Real type base
  class RealType : public Type {
    public:
      RealType() {}
      virtual ~RealType() {}
  };

  // flaot type base
  class FloatType : public RealType {
    public:
      FloatType() {}
      virtual ~FloatType() {}
  };

  // double type base
  class DoubleType : public RealType {
    public:
      DoubleType() {}
      virtual ~DoubleType() {}
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
