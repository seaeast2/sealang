
#include <assert.h>
#include "LocalVarResolver.h"

using namespace AST;


LocalVarResolver::LocalVarResolver() {
  decls_ = nullptr;
  currentScope_ = nullptr;
}

LocalVarResolver::~LocalVarResolver() {
}

void LocalVarResolver::PushScope() {
  currentScope_ = currentScope_->AddChild();
}

void LocalVarResolver::PopScope() {
  currentScope_ = currentScope_->GetParent() == nullptr ? currentScope_ : currentScope_->GetParent();
}


bool LocalVarResolver::Resolve(Declarations* decls, Scope* scp) {
  decls_ = decls;
  currentScope_ = scp;

  NamedDecl* name_decl = nullptr;

  // Collect global variables, functions, constants 
  for (int i = 0; i < decls_->GetFunctionDeclNum(); i++) {
    name_decl = decls_->GetFunctionDecl(i);
    if (!currentScope_->HasDuplicatedName(name_decl->GetName()))
      currentScope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error : There is a duplicated function name.");
  }

  for (int i = 0; i < decls_->GetConstantDeclNum(); i++) {
    name_decl = decls_->GetConstantDecl(i);
    if (!currentScope_->HasDuplicatedName(name_decl->GetName()))
      currentScope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  for (int i = 0; i < decls_->GetVariableDeclNum(); i++) {
    name_decl = decls_->GetVariableDecl(i);
    if (!currentScope_->HasDuplicatedName(name_decl->GetName()))
      currentScope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }


  ResolveVariableInitializer();
  ResolveConstantInitializer();
  ResolveFunction();

  // TODO : Need to deal with class memeber functions and 
  // class member variables
  
  return true;
}

bool LocalVarResolver::ResolveVariableInitializer() {
  // Traverse Variable initailizer's ExprNode
  for (int i = 0; i < decls_->GetVariableDeclNum(); i++) {
    if (ASTVisitor::Visit(decls_->GetVariableDecl(i)))
      return false;
  }

  return true;
}

bool LocalVarResolver::ResolveConstantInitializer() {
  // Traverse Constant initailizer's ExprNode
  for (int i = 0; i < decls_->GetConstantDeclNum(); i++) {
    if (ASTVisitor::Visit(decls_->GetConstantDecl(i)))
      return false;
  }

  return true;
}

bool LocalVarResolver::ResolveFunction() {
  // traverse function
  for (int i = 0; i < decls_->GetFunctionDeclNum(); i++) {
    if (Visit(decls_->GetFunctionDecl(i)))
      return false;
  }
}

bool LocalVarResolver::Visit(BlockNode* node) {
  PushScope();
  NamedDecl* nameDecl = nullptr;
  for (int i = 0; i < node->GetVarNum(); i++) {
    nameDecl = node->GetVariableDecl(i);
    if (!currentScope_->HasDuplicatedName(nameDecl->GetName()))
      currentScope_->AddNamedDecl(nameDecl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}

bool LocalVarResolver::Visit(VariableNode* node) {
  NamedDecl* nd = nullptr;
  if (node->IsFuncVarNode()) {
    // in case node is function name variable node
    // find Decl in function name's
    nd = currentScope_->FindFuncDecl(node->GetVarName());
  }
  else {
    // in case node is just variable node
    nd = currentScope_->FindDecl(node->GetVarName());
  }

  if (nd)
    node->SetNamedDecl(nd);
  else {
    // TODO : Add ErrorHandler and print detail information.
    assert(0&& "Error : Can't find variable definition.");
  }
  return ASTVisitor::Visit(node);
}

bool LocalVarResolver::Visit(FunctionDecl* node) {
  PushScope();
  NamedDecl* nameDecl = nullptr;
  // add param definition to scope
  for (int i = 0; i < node->GetParamNum(); i++) {
    nameDecl = node->GetParamNode(i);
    if (!currentScope_->HasDuplicatedName(nameDecl->GetName()))
      currentScope_->AddNamedDecl(nameDecl);
    else
      assert(0&& "Error : there is duplicated parameter name.");
  }

  // traverse function body
  bool res = Visit(node->GetBody());
  PopScope();
  return res;
}

