#ifndef _ast_type_h_
#define _ast_type_h_

#include <string.h>
using namespace std;

namespace AST {
  // These types are all AST Types.
  // Base type in AST
  class TypeName;

  class Type {
    public:
      enum TypeKind {
        BaseTy,
        VoidTy,
        IntTy,
        RealTy,
        NamedTy,
          CompositeTy,
            ClassTy,
          UserTy,
        PointerTy,
        FunctionTy
      };

    protected:
      TypeKind kind_;
    public:
      Type() {
        kind_ = BaseTy;
      };
      virtual ~Type() {};
      TypeKind GetKind() {
        return kind_;
      }
      virtual bool IsKindOf(TypeKind kind) = 0;
      virtual void Print();
  };

  class VoidType : public Type {
    public:
      VoidType() {
        kind_ = VoidTy;
      }
      virtual ~VoidType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == VoidTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class IntegerType : public Type {
    public:
      IntegerType() {
        kind_ = IntTy;
      }
      virtual ~IntegerType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == IntTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class RealType : public Type {
    public:
      RealType() {
        kind_ = RealTy;
      }
      virtual ~RealType() {}
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class NamedType : public Type {
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

  class TypeName {
    Type* type_;
    char  type_name_[256];
    public:
      TypeName() {
        type_ = NULL;
        memset(type_name_, 0, sizeof(type_name_));
      }
      TypeName(Type* ty, const char* type_name) {
        type_ = ty;
        strcpy(type_name_, type_name);
      }
      ~TypeName() {
      }
  };

}
#endif
