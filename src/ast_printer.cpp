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

// Statements
void ASTPrinter::visit(BlockNode* node){ 
  return;}
void ASTPrinter::visit(LabelNode* node){ return;}
void ASTPrinter::visit(ExprStmtNode* node){ return;}
void ASTPrinter::visit(IfNode* node){ return;}    
void ASTPrinter::visit(WhileNode* node){ return;}
void ASTPrinter::visit(DoWhileNode* node){ return;}
void ASTPrinter::visit(ForNode* node){ return;}
void ASTPrinter::visit(CaseNode* node){ return;}
void ASTPrinter::visit(SwitchNode* node){ return;}
void ASTPrinter::visit(BreakNode* node){ return;}
void ASTPrinter::visit(ContinueNode* node){ return;}
void ASTPrinter::visit(GotoNode* node){ return;}
void ASTPrinter::visit(ReturnNode* node){ return;}

// Expression
void ASTPrinter::visit(AssignNode* node){ return;}
void ASTPrinter::visit(OpAssignNode* node){ return;}
void ASTPrinter::visit(AddressNode* node){ return;}
void ASTPrinter::visit(BinaryOpNode* node){ return;}
void ASTPrinter::visit(LogicalAndNode* node){ return;}
void ASTPrinter::visit(LogicalOrNode* node){ return;}
void ASTPrinter::visit(CastNode* node){ return;}
void ASTPrinter::visit(CondExprNode* node){ return;}
void ASTPrinter::visit(FuncCallNode* node){ return;}
void ASTPrinter::visit(ArrayRefNode* node){ return;}
void ASTPrinter::visit(DereferenceNode* node){ return;}
void ASTPrinter::visit(MemberRefNode* node){ return;}
void ASTPrinter::visit(PtrMemberRefNode* node){ return;}
void ASTPrinter::visit(VariableNode* node){ return;}
void ASTPrinter::visit(IntegerLiteralNode* node){ return;}
void ASTPrinter::visit(RealLiteralNode* node){ return;}
void ASTPrinter::visit(StringLiteralNode* node){ return;}
void ASTPrinter::visit(SizeofExprNode* node){ return;}
void ASTPrinter::visit(SizeofTypeNode* node){ return;}
void ASTPrinter::visit(UnaryOpNode* node){ return;}
void ASTPrinter::visit(PrefixOpNode* node){ return;}   
void ASTPrinter::visit(SuffixOpNode* node){ return;}

// Etc
void ASTPrinter::visit(FunctionDecl* node){ 
  cout << "[FunctionDecl] : " << node->GetName() << endl;
  return;
}
void ASTPrinter::visit(VariableDecl* node){ return;}
void ASTPrinter::visit(ConstantDecl* node){ return;}
void ASTPrinter::visit(TypeNode* node){ return;} 
void ASTPrinter::visit(ParamNode* node){ return;}
void ASTPrinter::visit(ImportNode* node){ return;}
void ASTPrinter::visit(ArgsNode* node){ return;}        
void ASTPrinter::visit(ClassNode* node){ return;}   
void ASTPrinter::visit(TypedefNode* node) { 
  return;
} 
