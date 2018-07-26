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
bool ASTPrinter::visit(BlockNode* node){ 
  return true;}
bool ASTPrinter::visit(LabelNode* node){ return true;}
bool ASTPrinter::visit(ExprStmtNode* node){ return true;}
bool ASTPrinter::visit(IfNode* node){ return true;}    
bool ASTPrinter::visit(WhileNode* node){ return true;}
bool ASTPrinter::visit(DoWhileNode* node){ return true;}
bool ASTPrinter::visit(ForNode* node){ return true;}
bool ASTPrinter::visit(CaseNode* node){ return true;}
bool ASTPrinter::visit(SwitchNode* node){ return true;}
bool ASTPrinter::visit(BreakNode* node){ return true;}
bool ASTPrinter::visit(ContinueNode* node){ return true;}
bool ASTPrinter::visit(GotoNode* node){ return true;}
bool ASTPrinter::visit(ReturnNode* node){ return true;}

// Expression
bool ASTPrinter::visit(AssignNode* node){ return true;}
bool ASTPrinter::visit(OpAssignNode* node){ return true;}
bool ASTPrinter::visit(AddressNode* node){ return true;}
bool ASTPrinter::visit(BinaryOpNode* node){ return true;}
bool ASTPrinter::visit(LogicalAndNode* node){ return true;}
bool ASTPrinter::visit(LogicalOrNode* node){ return true;}
bool ASTPrinter::visit(CastNode* node){ return true;}
bool ASTPrinter::visit(CondExprNode* node){ return true;}
bool ASTPrinter::visit(FuncCallNode* node){ return true;}
bool ASTPrinter::visit(ArrayRefNode* node){ return true;}
bool ASTPrinter::visit(DereferenceNode* node){ return true;}
bool ASTPrinter::visit(MemberRefNode* node){ return true;}
bool ASTPrinter::visit(PtrMemberRefNode* node){ return true;}
bool ASTPrinter::visit(VariableNode* node){ return true;}
bool ASTPrinter::visit(IntegerLiteralNode* node){ return true;}
bool ASTPrinter::visit(RealLiteralNode* node){ return true;}
bool ASTPrinter::visit(StringLiteralNode* node){ return true;}
bool ASTPrinter::visit(SizeofExprNode* node){ return true;}
bool ASTPrinter::visit(SizeofTypeNode* node){ return true;}
bool ASTPrinter::visit(UnaryOpNode* node){ return true;}
bool ASTPrinter::visit(PrefixOpNode* node){ return true;}   
bool ASTPrinter::visit(SuffixOpNode* node){ return true;}

// Etc
bool ASTPrinter::visit(FunctionDecl* node){ 
  cout << "[FunctionDecl] : " << node->GetName() << endl;
  return true;
}
bool ASTPrinter::visit(VariableDecl* node){ return true;}
bool ASTPrinter::visit(ConstantDecl* node){ return true;}
bool ASTPrinter::visit(TypeNode* node){ return true;} 
bool ASTPrinter::visit(ParamNode* node){ return true;}
bool ASTPrinter::visit(ImportNode* node){ return true;}
bool ASTPrinter::visit(ArgsNode* node){ return true;}        
bool ASTPrinter::visit(ClassNode* node){ return true;}   
bool ASTPrinter::visit(TypedefNode* node) { 
  return true;
} 
