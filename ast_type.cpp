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
    Type* ty = NULL;
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
    Type* ty = NULL;
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
    Type* ty = NULL;
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
    Type* ty = NULL;
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
}

