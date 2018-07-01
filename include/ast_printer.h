#ifndef _ast_printer_h_
#define _ast_printer_h_

#include "visitor.h"
#include "ast_node.h"

namespace AST {

  // Print AST
  class ASTPrinter : public Visitor<void> {
    private:
      int depth_;
    public:
      ASTPrinter();
      virtual ~ASTPrinter();

      void Print(Declarations* decls);
      void Print(BaseNode* ast);

      // Statements
      virtual void visit(BlockNode* node);
      virtual void visit(LabelNode* node);
      virtual void visit(ExprStmtNode* node);
      virtual void visit(IfNode* node);    
      virtual void visit(WhileNode* node);
      virtual void visit(DoWhileNode* node);
      virtual void visit(ForNode* node);
      virtual void visit(CaseNode* node);
      virtual void visit(SwitchNode* node);
      virtual void visit(BreakNode* node);
      virtual void visit(ContinueNode* node);
      virtual void visit(GotoNode* node);
      virtual void visit(ReturnNode* node);

      // Expression
      virtual void visit(AssignNode* node);
      virtual void visit(OpAssignNode* node);
      virtual void visit(AddressNode* node);
      virtual void visit(BinaryOpNode* node);
      virtual void visit(LogicalAndNode* node);
      virtual void visit(LogicalOrNode* node);
      virtual void visit(CastNode* node);
      virtual void visit(CondExprNode* node);
      virtual void visit(FuncCallNode* node);
      virtual void visit(ArrayRefNode* node);
      virtual void visit(DereferenceNode* node);
      virtual void visit(MemberRefNode* node);
      virtual void visit(PtrMemberRefNode* node);
      virtual void visit(VariableNode* node);
      virtual void visit(IntegerLiteralNode* node);
      virtual void visit(RealLiteralNode* node);
      virtual void visit(StringLiteralNode* node);
      virtual void visit(SizeofExprNode* node);
      virtual void visit(SizeofTypeNode* node);
      virtual void visit(UnaryOpNode* node);
      virtual void visit(PrefixOpNode* node);   
      virtual void visit(SuffixOpNode* node);

      // Etc
      virtual void visit(FunctionDecl* node);
      virtual void visit(VariableDecl* node);
      virtual void visit(ConstantDecl* node);
      virtual void visit(TypeNode* node); 
      virtual void visit(ParamNode* node);
      virtual void visit(ImportNode* node);
      virtual void visit(ArgsNode* node);        
      virtual void visit(ClassNode* node);   
      virtual void visit(TypedefNode* node);  
  };
}

#endif
