#ifndef _ast_type_h_
#define _ast_type_h_

#include <string>
#include "core/simple_vector.h"
#include "core/list.h"

namespace AST {
  // These types are all AST Types.
  // Base type in AST

  class ASTContext;

  class Type {
    public:
      enum TypeKind {
        BaseTy,
          // primitive type
          VoidTy,
          IntegerTy,
            CharTy,
            ShortTy,
            IntTy,
            LongTy,
          RealTy,
            FloatTy,
            DoubleTy,

            // type that can have identifier.
          NamedTy,
            CompositeTy,
              RecordTy,
            UserTy,

          ArrayTy,
          PointerTy,
          FunctionTy,
          VarArgTy,
      };

    protected:
      TypeKind      kind_;
      std::string   type_name_;
      bool          isComplete_; // let us know if current is incomplete type.

      Type() {
        kind_ = BaseTy;
        type_name_ = "";
        isComplete_ = true;
      };

    public:
      virtual ~Type() {};
      TypeKind GetKind() {
        return kind_;
      }

      virtual bool IsKindOf(TypeKind kind) = 0;

      const char* GetTypeName() { return type_name_.c_str(); }
      bool IsComplete() { return isComplete_; }
      void Complete(bool ic) { isComplete_ = ic; }

      bool IsPrimitiveType() {
        if (IsKindOf(VoidTy) || IsKindOf(IntegerTy) || IsKindOf(RealTy))
          return true;
        return false;
      }
      bool IsRecordType() { return IsKindOf(RecordTy); }
      bool IsUserType() { return IsKindOf(UserTy); }
      bool IsArrayType() { return IsKindOf(ArrayTy); }
      bool IsPointerType() { return IsKindOf(PointerTy); }
      bool IsFunctionType() { return IsKindOf(FunctionTy); }

      virtual void Print();
      
      bool operator==(const Type & rhs) {
        return (this->kind_ == rhs.GetKind() && !strcmp(this->GetTypeName(), rhs.GetTypeName())); 
      }
  };

  typedef SimpleVector<Type*> Types;

  class VoidType : public Type {
    protected:
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
        type_name_ = "";
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
      SimpleList<Type*> member_types_;
    
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

      void AddMemberType(Type* mem_type) {
        member_types_.PushBack(mem_type);
      }

      bool RemoveMemberType(Type* mem_type) {
        return member_types_.Delete(mem_type);
      }

      Type* FindMemberType(Type* mem_type) { return member_types_.Find(mem_type); }

      int GetMemberNum() { return member_types_.GetSize(); }
      Type* GetMemberType(int index) { return member_types_.GetAt(index); }
  };

  class FunctionType : public Type {
    private:
      Type* return_type_;
      Types param_types_;
      Type* this_class_;

    protected:
      FunctionType() {
        kind_ = FunctionTy;
        this_class_ = nullptr;
      }

      FunctionType(Type* retty, Types const & param_types, Type* this_class = nullptr) {
        kind_ = FunctionTy;

        return_type_ = retty;
        param_types_ = param_types;
        this_class_ = this_class;
        
        type_name_ = MakeFnTypeName(retty, param_types, this_class);
      }

      static const char* MakeFnTypeName(Type* retTy, Types const & paramTypes, Type* thisClass);

    public:
      virtual ~FunctionType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == FunctionTy || kind == BaseTy)
          return true;
        return false;
      }

      Type* GetReturnType() { return return_type_; }
      int GetParamTypeNum() { return param_types_.GetSize(); }
      Type* GetParamType(int index) { 
        if (index < param_types_.GetSize())
          return param_types_[index]; 
        return nullptr;
      }
      Type* GetThisClassType() { return this_class_; }

      static FunctionType* Get(ASTContext* ac, Type* retty, Types const & param_types, Type* this_class = nullptr);
  };

  class RecordType : public CompositeType {
    private:
      SimpleList<FunctionType*> member_func_types_;
    
    protected:
      RecordType() {
        kind_ = RecordTy;
        type_name_ = "class type";
      }

      RecordType(const char* type_name) {
        kind_ = RecordTy;
        type_name_ = type_name;
      }
    public:
      virtual ~RecordType() {
      }

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == RecordTy || kind == CompositeTy || 
            kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }

      void AddMemberFuncType(FunctionType* mem_type) {
        member_func_types_.PushBack(mem_type);
      }

      bool RemoveMemberFuncType(FunctionType* mem_type) {
        return member_func_types_.Delete(mem_type);
      }

      Type* FindMemberFuncType(FunctionType* mem_type) { 
        return member_func_types_.Find(mem_type); }

      int GetMemberFuncNum() { return member_func_types_.GetSize(); }
      Type* GetMemberFuncType(int index) { return member_func_types_.GetAt(index); }

      static RecordType* Get(ASTContext* ac, const char* type_name); // create incomplete type
  };
  
  // typedef alias type 
  class UserType : public NamedType {
    private:
      Type* baseType_;

    protected:
      UserType() {
        kind_ = UserTy;

        baseType_ = nullptr;
      }

      UserType(Type* baseTy, const char* new_ty_name) {
        kind_ = UserTy;

        baseType_ = baseTy;
        type_name_ = new_ty_name;
      }
    public:
      virtual ~UserType() {}

      virtual bool IsKindOf(TypeKind kind) {
        if (kind == UserTy || kind == NamedTy || kind == BaseTy)
          return true;
        return false;
      }

      void SetBaseType(Type* baseTy) { baseType_ = baseTy; }
      Type* GetBaseType() { return baseType_; }

      static UserType* Get(ASTContext* ac, Type* baseTy, const char* alias); // unassigned array
  };

  class ArrayType : public Type {
    private:
      Type* base_type_;
      unsigned int array_size_;
    protected:
      ArrayType() {
        kind_ = ArrayTy;
      }
      ArrayType(Type* basety) {
        kind_ = ArrayTy;
        
        base_type_ = basety;
        type_name_ = basety->GetTypeName(); 
        type_name_ += "[]";
      }
      ArrayType(Type* basety, unsigned int size) {
        kind_ = ArrayTy;

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
      Type* GetBaseType() { return base_type_; }

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
      }
      PointerType(Type* basety) {
        kind_ = PointerTy;

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
  

  class VarArgType: public Type {
    protected:
      VarArgType() {
        type_name_ = "...";
        kind_ = VarArgTy;
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
