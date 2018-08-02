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
  if (!decls) {
    cout << "No Decls" << endl;
    return false;
  }

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
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "BlockNode" << endl;

  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(LabelNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "LabelNode" << endl;

  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(ExprStmtNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ExprStmtNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(IfNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "IfNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}    
bool ASTPrinter::Visit(WhileNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "WhileNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(DoWhileNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "DoWhileNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(ForNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ForNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(CaseNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "CaseNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(SwitchNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "SwitchNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(BreakNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "BreakNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(ContinueNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ContinueNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(GotoNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "GotoNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(ReturnNode* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ReturnNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

// Expression
bool ASTPrinter::Visit(AssignNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "AssignNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(OpAssignNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "OpAssignNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(AddressNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "AddressNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(BinaryOpNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  char* op = new char[3];
  memset(op, 0, sizeof(op));
  switch(node->GetOp()) {
    case BinaryOpNode::LogicOr:      // ||
      strcpy(op, "||");
      break;
    case BinaryOpNode::LogicAnd:     // &&
      strcpy(op, "&&");
      break;
    case BinaryOpNode::GreatorThan:  // >
      strcpy(op, ">");
      break;
    case BinaryOpNode::LessThan:     // <
      strcpy(op, "<");
      break;
    case BinaryOpNode::GreatorThanEq:// >=
      strcpy(op, ">=");
      break;
    case BinaryOpNode::LessThanEq:   // <=
      strcpy(op, "<=");
      break;
    case BinaryOpNode::Equal:        // ==
      strcpy(op, "==");
      break;
    case BinaryOpNode::NotEqual:     // !=
      strcpy(op, "!=");
      break;
    case BinaryOpNode::BitOr:        // |
      strcpy(op, "|");
      break;
    case BinaryOpNode::BitAnd:       // &
      strcpy(op, "&");
      break;
    case BinaryOpNode::BitXor:       // ^
      strcpy(op, "^");
      break;
    case BinaryOpNode::BitShiftLeft: // <<
      strcpy(op, "<<");
      break;
    case BinaryOpNode::BitShiftRight:// >>
      strcpy(op, ">>");
      break;
    case BinaryOpNode::BinSum:       // +
      strcpy(op, "+");
      break;
    case BinaryOpNode::BinSub:       // -
      strcpy(op, "-");
      break;
    case BinaryOpNode::BinMul:       // *
      strcpy(op, "*");
      break;
    case BinaryOpNode::BinDiv:       // /
      strcpy(op, "/");
      break;
    case BinaryOpNode::BinMod:       // %
      strcpy(op, "%");
      break;
    default:
      break;
  }

  cout << "BinaryOpNode : " << op << endl;
  delete[] op;

  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(LogicalAndNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";
  
  cout << "LogicalAndNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(LogicalOrNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "LogicalOrNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(CastNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "CastNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(CondExprNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "CondExprNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(FuncCallNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "FuncCallNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(ArrayRefNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ArrayRefNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(DereferenceNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "DereferenceNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(MemberRefNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "MemberRefNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(PtrMemberRefNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "PtrMemberRefNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(VariableNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "VariableNode : " << node->GetVarName() << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(IntegerLiteralNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "IntegerLiteralNode : " << node->GetValue() << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(RealLiteralNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "RealLiteralNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(StringLiteralNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "StringLiteralNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(SizeofExprNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "SizeofExprNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(SizeofTypeNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "SizeofTypeNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(UnaryOpNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "UnaryOpNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(PrefixOpNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "PrefixOpNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
   
bool ASTPrinter::Visit(SuffixOpNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "SuffixOpNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}


// Etc
bool ASTPrinter::Visit(FunctionDecl* node){ 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "FunctionDecl : " << node->GetName() << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
bool ASTPrinter::Visit(VariableDecl* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "VariableDecl : " << node->GetName() << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(ConstantDecl* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ConstantDecl" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(TypeNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  AST::Type* ty = node->GetType();
  if (ty)
    cout << "TypeNode : " << ty->GetTypeName() << endl;
  else
    cout << "TypeNode" << endl;

  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
 
bool ASTPrinter::Visit(ParamNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ParamNode : " << node->GetName() << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(ImportNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ImportNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}

bool ASTPrinter::Visit(ArgsNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ArgsNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
        
bool ASTPrinter::Visit(ClassNode* node){
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "ClassNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
}
   
bool ASTPrinter::Visit(TypedefNode* node) { 
  for (int i = 0; i < depth_; i++)
    cout << "  ";

  cout << "TypedefNode" << endl;
  depth_++;
  bool res = ASTVisitor::Visit(node);
  depth_--;
  return res;
} 
