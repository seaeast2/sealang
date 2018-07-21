#include <iostream>
#include <string>
#include "ast_type.h"
#include "astcontext.h"

using namespace std;

namespace AST {

  void Type::Print() {
    cout << "Base Type" << endl;
  }

  VoidType* VoidType::Get(ASTContext* ac) {
    Type* ty = ac->FindType("void");
    if (ty) 
      return (VoidType*)ty;

    ty = new VoidType();
    ac->AddType(ty->GetTypeName(), ty);
    return (VoidType*)ty;
  }

  CharType* CharType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->FindType("char");
    else 
      ty = ac->FindType("unsigned char");

    if (ty) 
      return (CharType*)ty;

    ty = new CharType(s);
    ac->AddType(ty->GetTypeName(), ty);
    return (CharType*)ty;
  }

  ShortType* ShortType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->FindType("short");
    else 
      ty = ac->FindType("unsigned short");
    if (ty) 
      return (ShortType*)ty;

    ty = new ShortType(s);
    ac->AddType(ty->GetTypeName(), ty);
    return (ShortType*)ty;
  }

  IntType* IntType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->FindType("int");
    else 
      ty = ac->FindType("unsigned int");
    if (ty) 
      return (IntType*)ty;

    ty = new IntType(s);
    ac->AddType(ty->GetTypeName(), ty);
    return (IntType*)ty;
  }

  LongType* LongType::Get(ASTContext* ac, IntegerType::eSign s) {
    Type* ty = nullptr;
    if (s == Signed)
      ty = ac->FindType("long");
    else 
      ty = ac->FindType("unsigned long");
    if (ty) 
      return (LongType*)ty;

    ty = new LongType(s);
    ac->AddType(ty->GetTypeName(), ty);
    return (LongType*)ty;
  }

  FloatType* FloatType::Get(ASTContext* ac) {
    Type* ty = ac->FindType("float");
    if (ty) 
      return (FloatType*)ty;

    ty = new FloatType();
    ac->AddType(ty->GetTypeName(), ty);
    return (FloatType*)ty;
  }

  DoubleType* DoubleType::Get(ASTContext* ac) {
    Type* ty = ac->FindType("double");
    if (ty) 
      return (DoubleType*)ty;

    ty = new DoubleType();
    ac->AddType(ty->GetTypeName(), ty);
    return (DoubleType*)ty;
  }

  // incomplete type
  ClassType* ClassType::Get(ASTContext* ac, const char* type_name) {
    Type* ty = ac->FindType(type_name);
    if (ty) 
      return (ClassType*)ty;

    ty = new ClassType(type_name);
    ty->Incomplete(true); // set with incomplete type.
    ac->AddType(ty->GetTypeName(), ty);
    return (ClassType*)ty;
  }

  ArrayType* ArrayType::Get(ASTContext* ac, Type* basety) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name += "[]";
    Type* ty = ac->FindType(arr_type_name.c_str());
    if (ty)
      return (ArrayType*) ty;

    ty = new ArrayType(basety);
    ac->AddType(ty->GetTypeName(), ty);
    return (ArrayType*) ty;
  }

  ArrayType* ArrayType::Get(ASTContext* ac, Type* basety, unsigned int size) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name = arr_type_name + "[" + to_string(size) + "]";
    Type* ty = ac->FindType(arr_type_name.c_str());
    if (ty)
      return (ArrayType*) ty;

    ty = new ArrayType(basety, size);
    ac->AddType(ty->GetTypeName(), ty);
    return (ArrayType*) ty;
  }

  PointerType* PointerType::Get(ASTContext* ac, Type* basety) {
    string arr_type_name = basety->GetTypeName();
    arr_type_name = arr_type_name + "*";
    Type* ty = ac->FindType(arr_type_name.c_str());
    if (ty)
      return (PointerType*) ty;

    ty = new PointerType(basety);
    ac->AddType(ty->GetTypeName(), ty);
    return (PointerType*) ty;
  }

  FunctionType* FunctionType::Get(ASTContext* ac, Type* retty, SimpleVector<Type*> param_types) {
    // funciton type typename
    // retty(paramty1,paramty2,paramty3,...) 
    string fn_type_name = retty->GetTypeName();
    fn_type_name += "(";
    for (int i = 0; i < param_types.GetSize(); i++) {
      fn_type_name += param_types[i]->GetTypeName();
      if (i+1 < param_types.GetSize())
        fn_type_name += ",";
    }
    fn_type_name += ")";
    Type* ty = ac->FindType(fn_type_name.c_str());
    if (ty)
      return (FunctionType*) ty;

    ty = new FunctionType(retty, param_types);
    ac->AddType(ty->GetTypeName(), ty);
    return (FunctionType*) ty;
  }


  VarArgType* VarArgType::Get(ASTContext* ac) {
    string vaarg = "...";
    Type* ty = ac->FindType(vaarg.c_str());
    if (ty)
      return (VarArgType*) ty;
    
    ty = new VarArgType();
    return (VarArgType*) ty;
  }
}
