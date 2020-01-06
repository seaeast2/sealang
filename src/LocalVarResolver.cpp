
#include <assert.h>
#include "LocalVarResolver.h"

using namespace AST;


LocalVarResolver::LocalVarResolver() {
  decls_ = nullptr;
  scope_ = nullptr;
  is_function_body_ = false;
}

LocalVarResolver::~LocalVarResolver() {
}

void LocalVarResolver::PushScope() {
  scope_ = scope_->AddChild();
}

void LocalVarResolver::PopScope() {
  scope_ = scope_->GetParent() == nullptr ? scope_ : scope_->GetParent();
}


bool LocalVarResolver::Check(Declarations* decls, Scope* scp) {
  decls_ = decls;
  scope_ = scp;

  NamedDecl* name_decl = nullptr;

  // Register global variable, function, cosntant
  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    name_decl = decls_->GetFunction(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  for (int i = 0; i < decls_->GetConstantNum(); i++) {
    name_decl = decls_->GetConstant(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  for (int i = 0; i < decls_->GetVariableNum(); i++) {
    name_decl = decls_->GetVariable(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
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

bool LocalVarResolver::Visit(BlockNode* node) {
  bool is_function_body = is_function_body_;
  if (!is_function_body)
    PushScope();
  is_function_body_ = false;

  NamedDecl* name_decl = nullptr;
  for (int i = 0; i < node->GetVarNum(); i++) {
    name_decl = node->GetVariable(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  bool res = ASTVisitor::Visit(node);
  if(!is_function_body)
    PopScope();
  return res;
}

bool LocalVarResolver::Visit(VariableNode* node) {
  NamedDecl* nd = scope_->FindDecl(node->GetVarName());
  if (nd)
    node->SetNamedDecl(nd);
  else {
    assert(0&& "Error on Variable checker : Can't find variable definition.");
  }
  return ASTVisitor::Visit(node);
}

bool LocalVarResolver::Visit(FunctionDecl* node) {
  PushScope();
  is_function_body_ = true;
  NamedDecl* name_decl = nullptr;
  for (int i = 0; i < node->GetParamNum(); i++) {
    name_decl = node->GetParamNode(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}

bool LocalVarResolver::Visit(ClassNode* node) {
  PushScope();
  NamedDecl* name_decl = nullptr;
  // Register class member variable, function
  for (int i = 0; i < node->GetMemVarNum(); i++) {
    name_decl = node->GetMemVariable(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }
  for (int i = 0; i < node->GetMemFunNum(); i++) {
    name_decl = node->GetMemFunction(i);
    if (!scope_->HasDuplicatedName(name_decl->GetName()))
      scope_->AddNamedDecl(name_decl);
    else
      assert(0&& "Error on Variable checker : Duplicate name");
  }

  // Traverse function body
  bool res = ASTVisitor::Visit(node);
  PopScope();
  return res;
}
