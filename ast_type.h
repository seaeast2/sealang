#ifndef _ast_type_h_
#define _ast_type_h_

#include <string>

namespace AST {
  // These types are all AST Types.
  // Base type in AST

  class ASTContext;

  class Type {
    public:
      enum TypeKind {
        BaseTy,
        VoidTy,
        IntegerTy,
          CharTy,
          ShortTy,
          IntTy,
          LongTy,
        RealTy,
          FloatTy,
          DoubleTy,
        NamedTy,
          CompositeTy,
            ClassTy,
          UserTy,
        PointerTy,
        FunctionTy
      };

    protected:
      TypeKind kind_;
      std::string type_name_;

      Type() {
        kind_ = BaseTy;
        type_name_ = "";
      };

    public:
      virtual ~Type() {};
      TypeKind GetKind() {
        return kind_;
      }

      virtual bool IsKindOf(TypeKind kind) = 0;
      virtual const char* GetTypeName() = 0;
      virtual void Print();
  };

  class VoidType : public Type {
    protected:
      VoidType() {
        kind_ = VoidTy;
        type_name_ = "void";
      }
    public:
      virtual ~VoidType() {}

      static const VoidType* Get(ASTContext* ac); // Type creator

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == VoidTy || kind == BaseTy)
          return true;
        return false;
      }
      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class IntegerType: public Type {
    public:
      enum eSign {
        Signed,
        Unsigned,
      };

    protected:
      eSign sign_;

      IntegerType() {
        sign_ = Signed;
      }

      IntegerType(eSign sign) {
        kind_ = IntegerTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "signed integer";
        else
          type_name_ = "unsigned integer";
      }
    public:

      virtual ~IntegerType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }
      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }

      eSign GetSign() {
        return sign_;
      }
  };

  // 8bit 
  class CharType: public IntegerType {
    protected:
      CharType(IntegerType::eSign sign) {
        kind_ = CharTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "char";
        else
          type_name_ = "unsigned char";
      }
    public:
      virtual ~CharType() {}

      static const CharType* Get(ASTContext* ac);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == CharTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }
      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  // 16bit
  class ShortType: public IntegerType {
    protected:
      ShortType(IntegerType::eSign sign) {
        kind_ = ShortTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "short";
        else
          type_name_ = "unsigned short";
      }
    public:
      virtual ~ShortType() {}

      static const ShortType* Get(ASTContext* ac);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ShortTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };
  
  // 32bit
  class IntType: public IntegerType {
    protected:
      IntType(IntegerType::eSign sign) {
        kind_ = IntTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "int";
        else
          type_name_ = "unsigned int";
      }
    public:
      virtual ~IntType() {}

      static const IntType* Get(ASTContext* ac);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == IntTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  // 64bit
  class LongType: public IntegerType {
    protected:
      LongType(IntegerType::eSign sign) {
        kind_ = LongTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "long";
        else
          type_name_ = "unsigned long";
      }
    public:
      virtual ~LongType() {}

      static const LongType* Get(ASTContext* ac);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == LongTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class RealType : public Type {
    protected:
      RealType() {
        kind_ = RealTy;
        type_name_ = "real";
      }
    public:
      virtual ~RealType() {}
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class FloatType : public RealType {
    protected:
      FloatType() {
        kind_ = FloatTy;
        type_name_ = "float";
      }
    public:
      virtual ~FloatType() {}

      static const FloatType* Get(ASTContext* ac);
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FloatTy || kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class DoubleType : public RealType {
    protected:
      DoubleType() {
        kind_ = DoubleTy;
        type_name_ = "double";
      }
    public:
      virtual ~DoubleType() {}

      static const DoubleType* Get(ASTContext* ac);
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == DoubleTy || kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class NamedType : public Type {
    protected:
      NamedType() {
        kind_ = NamedTy;
        type_name_ = "named type";
      }
    public:
      virtual ~NamedType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class CompositeType : public NamedType {
    protected:
      CompositeType() {
        kind_ = CompositeTy;
      }
    public:
      virtual ~CompositeType() {}
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == CompositeTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class ClassType : public CompositeType {
    protected:
      ClassType() {
        kind_ = ClassTy;
      }
    public:
      virtual ~ClassType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ClassTy || kind == CompositeTy || 
            kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class UserType : public NamedType {
    protected:
      UserType() {
        kind_ = UserTy;
      }
    public:
      virtual ~UserType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == UserTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class PointerType : public Type {
    protected:
      PointerType() {
        kind_ = PointerTy;
      }
    public:
      virtual ~PointerType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == PointerTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class FunctionType : public Type {
    protected:
      FunctionType() {
        kind_ = FunctionTy;
      }
    public:
      virtual ~FunctionType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FunctionTy || kind == BaseTy)
          return true;
        return false;
      }
  };

}
#endif
