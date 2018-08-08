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

  return true;
}
