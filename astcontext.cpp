#include "astcontext.h"
#include <iostream>

using namespace std;

namespace AST {
  ASTContext::ASTContext() {
    decls_ = new Declarations();
  }

  ASTContext::~ASTContext() {
    delete decls_;
  }

  void ASTContext::AddImport(const std::string &import) {
    imports_.push_back(import);
  }

  VecStr::iterator ASTContext::import_begin() {
    return imports_.begin();
  }

  VecStr::iterator ASTContext::import_end() {
    return imports_.end();
  }

  Type* ASTContext::FindType(const char* type_name) {
    return type_env_.GetValue(type_name);
  }

  bool ASTContext::AddType(const char* key, Type* type) {
    return type_env_.Push(key, type);
  }

  Type* ASTContext::RemoveType(const char* key) {
    Type* ty = type_env_.GetValue(key);
    type_env_.Pop(key);
    return ty;
  }

  void ASTContext::PrintImports() {
    VecStr::iterator  i = import_begin(), e = import_end();
    for (;i != e; i++) {
      cout << *i << endl;
    }
  }
};
