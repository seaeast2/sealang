#include <iostream>
#include <string>
#include "ast_printer.h"

using namespace std;
using namespace AST;


ASTPrinter::ASTPrinter() {
  depth_ = 0;
}

ASTPrinter::~ASTPrinter() {
}

bool ASTPrinter::Print(Declarations* decls) {
  for (int i = 0; i < decls->GetFunctionNum(); i++) {
    if(!Visit(decls->GetFunction(i)))
      return false;
  }
  for (int i = 0; i < decls->GetConstantNum(); i++) {
    if(!Visit(decls->GetConstant(i)))
      return false;
  }
  for (int i = 0; i < decls->GetVariableNum(); i++) {
    if(!Visit(decls->GetVariable(i)))
      return false;
  }
  for (int i = 0; i < decls->GetClassNum(); i++) {
    if(!Visit(decls->GetClass(i)))
      return false;
  }
  for (int i = 0; i < decls->GetTypedefNum(); i++) {
    if(!Visit(decls->GetTypedef(i)))
      return false;
  }
  for (int i = 0; i < decls->GetImportNum(); i++) {
    if(!Visit(decls->GetImport(i)))
      return false;
  }
}

// Statements
bool ASTPrinter::Visit(BlockNode* node){ 
  cout << "BlockNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(LabelNode* node){ 
  cout << "LabelNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(ExprStmtNode* node){ 
  cout << "ExprStmtNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(IfNode* node){ 
  cout << "IfNode" << endl;
  return ASTVisitor::Visit(node);
}    
bool ASTPrinter::Visit(WhileNode* node){ 
  cout << "WhileNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(DoWhileNode* node){ 
  cout << "DoWhileNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(ForNode* node){ 
  cout << "ForNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(CaseNode* node){ 
  cout << "CaseNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(SwitchNode* node){ 
  cout << "SwitchNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(BreakNode* node){ 
  cout << "BreakNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(ContinueNode* node){ 
  cout << "ContinueNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(GotoNode* node){ 
  cout << "GotoNode" << endl;
  return ASTVisitor::Visit(node);
}
bool ASTPrinter::Visit(ReturnNode* node){ 
  cout << "ReturnNode" << endl;
  return ASTVisitor::Visit(node);
}

// Expression
bool ASTPrinter::Visit(AssignNode* node){
  cout << "AssignNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(OpAssignNode* node){
  cout << "OpAssignNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(AddressNode* node){
  cout << "AddressNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(BinaryOpNode* node){
  cout << "BinaryOpNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(LogicalAndNode* node){
  cout << "LogicalAndNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(LogicalOrNode* node){
  cout << "LogicalOrNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(CastNode* node){
  cout << "CastNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(CondExprNode* node){
  cout << "CondExprNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(FuncCallNode* node){
  cout << "FuncCallNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(ArrayRefNode* node){
  cout << "ArrayRefNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(DereferenceNode* node){
  cout << "DereferenceNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(MemberRefNode* node){
  cout << "MemberRefNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(PtrMemberRefNode* node){
  cout << "PtrMemberRefNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(VariableNode* node){
  cout << "VariableNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(IntegerLiteralNode* node){
  cout << "IntegerLiteralNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(RealLiteralNode* node){
  cout << "RealLiteralNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(StringLiteralNode* node){
  cout << "StringLiteralNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(SizeofExprNode* node){
  cout << "SizeofExprNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(SizeofTypeNode* node){
  cout << "SizeofTypeNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(UnaryOpNode* node){
  cout << "UnaryOpNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(PrefixOpNode* node){
  cout << "PrefixOpNode" << endl;
  return ASTVisitor::Visit(node);
}
   
bool ASTPrinter::Visit(SuffixOpNode* node){
  cout << "SuffixOpNode" << endl;
  return ASTVisitor::Visit(node);
}


// Etc
bool ASTPrinter::Visit(FunctionDecl* node){ 
  cout << "FunctionDecl" << endl;
  return true;
}
bool ASTPrinter::Visit(VariableDecl* node){
  cout << "VariableDecl" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(ConstantDecl* node){
  cout << "ConstantDecl" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(TypeNode* node){
  cout << "TypeNode" << endl;
  return ASTVisitor::Visit(node);
}
 
bool ASTPrinter::Visit(ParamNode* node){
  cout << "ParamNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(ImportNode* node){
  cout << "ImportNode" << endl;
  return ASTVisitor::Visit(node);
}

bool ASTPrinter::Visit(ArgsNode* node){
  cout << "ArgsNode" << endl;
  return ASTVisitor::Visit(node);
}
        
bool ASTPrinter::Visit(ClassNode* node){
  cout << "ClassNode" << endl;
  return ASTVisitor::Visit(node);
}
   
bool ASTPrinter::Visit(TypedefNode* node) { 
  cout << "TypedefNode" << endl;
  return ASTVisitor::Visit(node);
} 
