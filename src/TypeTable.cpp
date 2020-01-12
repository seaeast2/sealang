#include "TypeTable.h"

using namespace AST;


TypeTable::TypeTable() {
}
      
TypeTable::~TypeTable() {
}

bool TypeTable::Add(Type* ty) {
    if (table_.Find(ty->GetTypeName()))
      return false;
    table_.Insert(ty->GetTypeName(), ty);
    return true;
}

bool TypeTable::Remove(const char* typeName) {
    if (!table_.Find(typeName))
      return false;
    return table_.Delete(typeName);
}

Type* TypeTable::Find(const char* typeName) {
  return table_.Find(typeName);
}

void TypeTable::ResetItr() {
  table_.ResetItr();
}

Type* TypeTable::Next() {
  return table_.Next();
}
