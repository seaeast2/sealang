#include <iostream>
#include <string>
#include <assert.h>
#include "ASTType.h"
#include "ASTContext.h"

using namespace std;

namespace AST {

  void Type::Print() {
    cout << "Base Type" << endl;
  }

  VoidType* VoidType::Get(ASTContext* ac) {
    Type* ty = ac->GetType("void");
    if (ty) 
      return (VoidType*)ty;

    ty = new VoidType();
    ac->AddType(ty);
    return (VoidType*)ty;
  }

  CharType* CharType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->GetType("char");
    else 
      ty = ac->GetType("unsigned char");

    if (ty) 
      return (CharType*)ty;

    ty = new CharType(s);
    ac->AddType(ty);
    return (CharType*)ty;
  }

  ShortType* ShortType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->GetType("short");
    else 
      ty = ac->GetType("unsigned short");
    if (ty) 
      return (ShortType*)ty;

    ty = new ShortType(s);
    ac->AddType(ty);
    return (ShortType*)ty;
  }

  IntType* IntType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->GetType("int");
    else 
      ty = ac->GetType("unsigned int");
    if (ty) 
      return (IntType*)ty;

    ty = new IntType(s);
    ac->AddType(ty);
    return (IntType*)ty;
  }

  LongType* LongType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->GetType("long");
    else 
      ty = ac->GetType("unsigned long");
    if (ty) 
      return (LongType*)ty;

    ty = new LongType(s);
    ac->AddType(ty);
    return (LongType*)ty;
  }

  FloatType* FloatType::Get(ASTContext* ac) {
    Type* ty = ac->GetType("float");
    if (ty) 
      return (FloatType*)ty;

    ty = new FloatType();
    ac->AddType(ty);
    return (FloatType*)ty;
  }

  DoubleType* DoubleType::Get(ASTContext* ac) {
    Type* ty = ac->GetType("double");
    if (ty) 
      return (DoubleType*)ty;

    ty = new DoubleType();
    ac->AddType(ty);
    return (DoubleType*)ty;
  }

  RecordType* RecordType::Get(ASTContext* ac, const char* type_name) {
    Type* ty = ac->GetType(type_name);
    if (ty) 
      return (RecordType*)ty;

    ty = new RecordType(type_name);
    ty->Complete(false); // set as incomplete
    ac->AddType(ty);
    return (RecordType*)ty;
  }

  UserType* UserType::Get(ASTContext* ac, Type* original_type, const char* alias) {
    Type* ty = ac->GetType(alias);
    if (ty) {
      if (!ty->IsKindOf(Type::UserTy)) {
        assert(0 && "already defined type name.");
        return nullptr;
      }
      return (UserType*) ty;
    }

    ty = new UserType(original_type, alias);
    ac->AddType(ty);
    return (UserType*)ty;
  }

  ArrayType* ArrayType::Get(ASTContext* ac, Type* basety) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name += "[]";
    Type* ty = ac->GetType(arr_type_name.c_str());
    if (ty)
      return (ArrayType*) ty;

    ty = new ArrayType(basety);
    ac->AddType(ty);
    return (ArrayType*) ty;
  }

  ArrayType* ArrayType::Get(ASTContext* ac, Type* basety, unsigned int size) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name = arr_type_name + "[" + to_string(size) + "]";
    Type* ty = ac->GetType(arr_type_name.c_str());
    if (ty)
      return (ArrayType*) ty;

    ty = new ArrayType(basety, size);
    ac->AddType(ty);
    return (ArrayType*) ty;
  }

  PointerType* PointerType::Get(ASTContext* ac, Type* basety) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name = arr_type_name + "*";
    Type* ty = ac->GetType(arr_type_name.c_str());
    if (ty)
      return (PointerType*) ty;

    ty = new PointerType(basety);
    ac->AddType(ty);
    return (PointerType*) ty;
  }

  FunctionType* FunctionType::Get(ASTContext* ac, Type* retty, Types const & param_types, Type* this_class) {
    
    Type* ty = ac->GetType(FunctionType::MakeFnTypeName(retty, param_types, this_class));
    if (ty)
      return (FunctionType*) ty;

    ty = new FunctionType(retty, param_types);
    ty->Complete(false); // set as incomplete
    ac->AddType(ty);
    return (FunctionType*) ty;
  }

  const char* FunctionType::MakeFnTypeName(Type* retTy, Types const & paramTypes, Type* thisClass) {
    // funciton type typename
    // standard type : 
    //        retty(paramty1,paramty2,paramty3,...) 
    // class member function type : 
    //        retty(self class_name,paramty1,paramty2,paramty3,...) 
    
    // 1. add return type
    static std::string funcTypeName = retTy->GetTypeName();
    // 2. set this class
    funcTypeName += "(";
    if (thisClass) {
      funcTypeName += "self ";
      funcTypeName += thisClass->GetTypeName();
      funcTypeName += ",";
    }
    // 3. add parameter type
    for (int i = 0; i < paramTypes.GetSize(); i++) {
      funcTypeName += paramTypes[i]->GetTypeName();
      if (i < paramTypes.GetSize()-1)
        funcTypeName += ",";
    }
    funcTypeName += ")";

    return funcTypeName.c_str();
  }

  VarArgType* VarArgType::Get(ASTContext* ac) {
    string vaarg = "...";
    Type* ty = ac->GetType(vaarg.c_str());
    if (ty)
      return (VarArgType*) ty;
    
    ty = new VarArgType();
    return (VarArgType*) ty;
  }
}

