#include <assert.h>
#include "type_resolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
  decls_ = nullptr;
  type_table_ = nullptr;
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::Check(Declarations* decl, TypeTable* tt) {
  decls_ = decl;
  type_table_ = tt;

  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    if ((decls_->GetFunction(i)))
      return false;
  }

  for (int i = 0; i < decls_->GetClassNum(); i++) {
    if ((decls_->GetClass(i)))
      return false;
  }

  return true;
}
