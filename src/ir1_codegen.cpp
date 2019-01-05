#include "ast_node.h"
#include "ir1_codegen.h"
#include "ir1_instructions.h"

using namespace IR;

bool CodeGen::GenerateIR(AST::Declarations* decl) {
  for (int i = 0; i < decl->GetVariableNum(); i++) {
    AST::VariableDecl* vd = decl->GetVariable(i);
    Visit(vd);
  }

  for (int i = 0; i < decl->GetFunctionNum(); i++) {
    AST::FunctionDecl* fd = decl->GetFunction(i);
    Visit(fd);
  }

  return true;
}

bool CodeGen::Visit(AST::IfNode* node) {
  return true;
}

bool CodeGen::Visit(AST::FunctionDecl* node) {
  return true;
}

bool CodeGen::Visit(AST::VariableDecl* node) {
  return true;
}
