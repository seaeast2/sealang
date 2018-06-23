#ifndef _ast_type_h_
#define _ast_type_h_

#include <string>
#include "core/simple_vector.h"

namespace AST {
  // These types are all AST Types.
  // Base type in AST

  class ASTContext;
  class Type;

  typedef SimpleVector<Type*> Types;

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
        ArrayTy,
        PointerTy,
        FunctionTy,
        VarArgTy,
      };

    protected:
      TypeKind      kind_;
      std::string   type_name_;
      bool          is_incomplete_; // let us know if current is incomplete type.

      Type() {
        kind_ = BaseTy;
        type_name_ = "";
        is_incomplete_ = false;
      };

    public:
      virtual ~Type() {};
      TypeKind GetKind() {
        return kind_;
      }

      virtual bool IsKindOf(TypeKind kind) = 0;

      const char* GetTypeName() { return type_name_.c_str(); }
      bool IsIncomplete() { return is_incomplete_; }
      void Incomplete(bool ic) { is_incomplete_ = ic; }

      virtual void Print();
  };

  class VoidType : public Type {
    protected:
      VoidType() {
        kind_ = VoidTy;
        type_name_ = "void";
        is_incomplete_ = false;
      }
    public:
      virtual ~VoidType() {}

      static VoidType* Get(ASTContext* ac); // Type creator

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == VoidTy || kind == BaseTy)
          return true;
        return false;
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
        is_incomplete_ = false;
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
        is_incomplete_ = false;
      }
    public:
      virtual ~CharType() {}

      static CharType* Get(ASTContext* ac, IntegerType::eSign s);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == CharTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
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
        is_incomplete_ = false;
      }
    public:
      virtual ~ShortType() {}

      static ShortType* Get(ASTContext* a, IntegerType::eSign sc);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ShortTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
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
        is_incomplete_ = false;
      }
    public:
      virtual ~IntType() {}

      static IntType* Get(ASTContext* ac, IntegerType::eSign s);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == IntTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
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
        is_incomplete_ = false;
      }
    public:
      virtual ~LongType() {}

      static LongType* Get(ASTContext* ac, IntegerType::eSign s);

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == LongTy || kind == IntegerTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class RealType : public Type {
    protected:
      RealType() {
        kind_ = RealTy;
        type_name_ = "real";
        is_incomplete_ = false;
      }
    public:
      virtual ~RealType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class FloatType : public RealType {
    protected:
      FloatType() {
        kind_ = FloatTy;
        type_name_ = "float";
        is_incomplete_ = false;
      }
    public:
      virtual ~FloatType() {}

      static FloatType* Get(ASTContext* ac);
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FloatTy || kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class DoubleType : public RealType {
    protected:
      DoubleType() {
        kind_ = DoubleTy;
        type_name_ = "double";
        is_incomplete_ = false;
      }
    public:
      virtual ~DoubleType() {}

      static DoubleType* Get(ASTContext* ac);
      
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == DoubleTy || kind == RealTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class NamedType : public Type {
    protected:
      NamedType() {
        kind_ = NamedTy;
        type_name_ = "named type";
        is_incomplete_ = false;
      }
    public:
      virtual ~NamedType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class CompositeType : public NamedType {
    protected:
      CompositeType() {
        kind_ = CompositeTy;
        is_incomplete_ = false;
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
        type_name_ = "class type";
        is_incomplete_ = false;
      }

      ClassType(const char* type_name) {
        kind_ = ClassTy;
        type_name_ = type_name;
        is_incomplete_ = false;
      }
    public:
      virtual ~ClassType() {
      }

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ClassTy || kind == CompositeTy || 
            kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }

      static ClassType* Get(ASTContext* ac, const char* type_name); // create incomplete type
      //static ClassType* Get(ASTContext* ac); // TODO : create complete type
  };

  class UserType : public NamedType {
    protected:
      UserType() {
        kind_ = UserTy;
        is_incomplete_ = false;
      }
    public:
      virtual ~UserType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == UserTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }
  };

  class ArrayType : public Type {
    private:
      Type* base_type_;
      unsigned int array_size_;
    protected:
      ArrayType() {
        kind_ = ArrayTy;
        is_incomplete_ = false;
      }
      ArrayType(Type* basety) {
        kind_ = ArrayTy;
        is_incomplete_ = false;
        
        base_type_ = basety;
        type_name_ = basety->GetTypeName(); 
        type_name_ += "[]";
      }
      ArrayType(Type* basety, unsigned int size) {
        kind_ = ArrayTy;
        is_incomplete_ = false;

        base_type_ = basety;
        array_size_= size;
        type_name_ = basety->GetTypeName();
        type_name_ = type_name_ + "[" + std::to_string(size) + "]";
      }
    public:
      virtual ~ArrayType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == ArrayTy || kind == BaseTy)
          return true;
        return false;
      }
      static ArrayType* Get(ASTContext* ac, Type* basety); // unassigned array
      static ArrayType* Get(ASTContext* ac, Type* basety, unsigned int size); // assigned array
  };

  class PointerType : public Type {
    private:
      Type* base_type_;
    protected:
      PointerType() {
        base_type_ = nullptr;
        kind_ = PointerTy;
        is_incomplete_ = false;
      }
      PointerType(Type* basety) {
        kind_ = PointerTy;
        is_incomplete_ = false;

        base_type_ = basety;
        type_name_ = basety->GetTypeName();
        type_name_ += "*";
      }
    public:
      virtual ~PointerType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == PointerTy || kind == BaseTy)
          return true;
        return false;
      }

      static PointerType* Get(ASTContext* ac, Type* basety);
  };
  
  class FunctionType : public Type {
    private:
      Type* return_type_;
      SimpleVector<Type*> param_types_;
    protected:
      FunctionType() {
        kind_ = FunctionTy;
        is_incomplete_ = false;
      }

      FunctionType(Type* retty, SimpleVector<Type*> param_types) {
        kind_ = FunctionTy;
        is_incomplete_ = false;

        return_type_ = retty;
        param_types_ = param_types;
        
        // funciton type typename
        // retty(paramty1,paramty2,paramty3,...) 
        std::string fn_type_name = retty->GetTypeName();
        fn_type_name += "(";
        for (int i = 0; i < param_types.GetSize(); i++) {
          fn_type_name += param_types[i]->GetTypeName();
          if (i+1 < param_types.GetSize())
            fn_type_name += ",";
        }
        fn_type_name += ")";
        type_name_ = fn_type_name;
      }
    public:
      virtual ~FunctionType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FunctionTy || kind == BaseTy)
          return true;
        return false;
      }

      static FunctionType* Get(ASTContext* ac, Type* retty, SimpleVector<Type*> param_types);
  };

  class VarArgType: public Type {
    protected:
      VarArgType() {
        type_name_ = "...";
        kind_ = VarArgTy;
        is_incomplete_ = false;
      }
    public:
      virtual ~VarArgType() {}
      virtual bool IsKindOf(TypeKind kind) {
        if (kind == VarArgTy || kind == BaseTy)
          return true;
        return false;
      }
      
      static VarArgType* Get(ASTContext* ac);
  };


}

#endif
