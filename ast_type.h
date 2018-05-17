#ifndef _ast_type_h_
#define _ast_type_h_

#include <string>
#include "astcontext.h"

namespace AST {
  // These types are all AST Types.
  // Base type in AST

  class Type {
    public:
      enum TypeKind {
        BaseTy,
        VoidTy,
        IntegerType,
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

    public:
      Type() {
        kind_ = BaseTy;
        type_name_ = "";
      };
      virtual ~Type() {};
      TypeKind GetKind() {
        return kind_;
      }

      virtual bool IsKindOf(TypeKind kind) = 0;
      virtual const char* GetTypeName() = 0;
      virtual void Print();
  };

  class VoidType : public Type {
      VoidType() {
        kind_ = VoidTy;
        type_name_ = "void";
      }
    public:
      virtual ~VoidType() {}

      static VoidType* Get(ASTContext* ac); // Type creator

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
    private:
      eSign sign_;

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
  class CharType: public Type {
      CharType(eSign sign) {
        kind_ = CharTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "char";
        else
          type_name_ = "unsigned char";
      }
    public:
      virtual ~CharType() {}

      static CharType* Get(ASTContext* ac);

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
  class ShortType: public Type {
    public:
      ShortType(eSign sign) {
        kind_ = ShortTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "short";
        else
          type_name_ = "unsigned short";
      }
      virtual ~ShortType() {}

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
  class IntType: public Type {
    public:
      IntType(eSign sign) {
        kind_ = IntTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "int";
        else
          type_name_ = "unsigned int";
      }
      virtual ~IntType() {}

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
  class LongType: public Type {
    public:
      LongType(eSign sign) {
        kind_ = LongTy;
        sign_ = sign;
        if (sign_ == Signed)
          type_name_ = "long";
        else
          type_name_ = "unsigned long";
      }
      virtual ~LongType() {}

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
    public:
      RealType() {
        kind_ = RealTy;
        type_name_ = "real";
      }
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

  class FloatType : public Type {
    public:
      FloatType() {
        kind_ = FloatTy;
        type_name_ = "float";
      }
      virtual ~FloatType() {}
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FloatTy || kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }

      virtual const char* GetTypeName() {
        return type_name_.c_str();
      }
  };

  class DoubleType : public Type {
    public:
      DoubleType() {
        kind_ = DoubleTy;
        type_name_ = "double";
      }
      virtual ~DoubleType() {}
      
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
    char name_[64]; // type name
    public:
      NamedType() {
        kind_ = NamedTy;
      }
      virtual ~NamedType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class CompositeType : public NamedType {
    public:
      CompositeType() {
        kind_ = CompositeTy;
      }
      virtual ~CompositeType() {}
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == CompositeTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class ClassType : public CompositeType {
    public:
      ClassType() {
        kind_ = ClassTy;
      }
      virtual ~ClassType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ClassTy || kind == CompositeTy || 
            kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class UserType : public NamedType {
    public:
      UserType() {
        kind_ = UserTy;
      }
      virtual ~UserType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == UserTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class PointerType : public Type {
    public:
      PointerType() {
        kind_ = PointerTy;
      }
      virtual ~PointerType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == PointerTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class FunctionType : public Type {
    public:
      FunctionType() {
        kind_ = FunctionTy;
      }
      virtual ~FunctionType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FunctionTy || kind == BaseTy)
          return true;
        return false;
      }
  };

}
#endif
