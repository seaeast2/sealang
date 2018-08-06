
#include <assert.h>
#include "local_var_checker.h"

using namespace AST;


LocalVarChecker::LocalVarChecker() {
}

LocalVarChecker::~LocalVarChecker() {
}

void LocalVarChecker::PushScope() {
  scope_ = scope_->AddChild();
}

void LocalVarChecker::PopScope() {
  scope_ = scope_->GetParent() == nullptr ? scope_ : scope_->GetParent();
}


bool LocalVarChecker::Check(Declarations* decls, Scope* scp) {
  decls_ = decls;
  scope_ = scp;

  NamedDecl* name_decl = nullptr;

  // Register global variable, function, cosntant
  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    name_decl = decls_->GetFunction(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  for (int i = 0; i < decls_->GetConstantNum(); i++) {
    name_decl = decls_->GetConstant(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  for (int i = 0; i < decls_->GetVariableNum(); i++) {
    name_decl = decls_->GetVariable(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }


  // Traverse function body
  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    if (Visit(decls_->GetFunction(i)))
      return false;
  }

  // Traverse Class
  for (int i = 0; i < decls_->GetClassNum(); i++) {
    if (Visit(decls_->GetClass(i)))
      return false;
  }
  
  return true;
}

bool LocalVarChecker::Visit(BlockNode* node) {
  PushScope();
  NamedDecl* name_decl = nullptr;
  for (int i = 0; i < node->GetVarNum(); i++) {
    name_decl = node->GetVariable(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}

bool LocalVarChecker::Visit(VariableNode* node) {
  NamedDecl* nd = scope_->GetDecl(node->GetVarName());
  if (nd)
    node->SetNamedDecl(nd);
  else {
    assert(0&& "Error on Variable checker : Can't find variable definition.");
  }
  return ASTVisitor::Visit(node);
}

bool LocalVarChecker::Visit(FunctionDecl* node) {
  PushScope();
  NamedDecl* name_decl = nullptr;
  for (int i = 0; i < node->GetParamNum(); i++) {
    name_decl = node->GetParamNode(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}

bool LocalVarChecker::Visit(ClassNode* node) {
  PushScope();
  NamedDecl* name_decl = nullptr;
  // Register class member variable, function
  for (int i = 0; i < node->GetMemVarNum(); i++) {
    name_decl = node->GetMemVariable(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }
  for (int i = 0; i < node->GetMemFunNum(); i++) {
    name_decl = node->GetMemFunction(i);
    if (!scope_->IsDuplicatedNameInCurScope(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  // Traverse function body
  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}
