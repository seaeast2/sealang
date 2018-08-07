#include <assert.h>
#include "type_checker.h"

using namespace AST;

TypeChecker::TypeChecker() {
  decls_ = nullptr;
}

TypeChecker::~TypeChecker() {
}
