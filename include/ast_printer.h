#ifndef _ast_printer_h_
#define _ast_printer_h_

#include "visitor_base.h"
#include "ast_node.h"

namespace AST {

  // Print AST
  class ASTPrinter : public VisitorBase {
    private:
      int depth_;
    public:
      ASTPrinter();
      virtual ~ASTPrinter();

      bool Print(Declarations* decls);
      bool Print(BaseNode* ast);

      // Statements
      virtual bool visit(BlockNode* node);
      virtual bool visit(LabelNode* node);
      virtual bool visit(ExprStmtNode* node);
      virtual bool visit(IfNode* node);    
      virtual bool visit(WhileNode* node);
      virtual bool visit(DoWhileNode* node);
      virtual bool visit(ForNode* node);
      virtual bool visit(CaseNode* node);
      virtual bool visit(SwitchNode* node);
      virtual bool visit(BreakNode* node);
      virtual bool visit(ContinueNode* node);
      virtual bool visit(GotoNode* node);
      virtual bool visit(ReturnNode* node);

      // Expression
      virtual bool visit(AssignNode* node);
      virtual bool visit(OpAssignNode* node);
      virtual bool visit(AddressNode* node);
      virtual bool visit(BinaryOpNode* node);
      virtual bool visit(LogicalAndNode* node);
      virtual bool visit(LogicalOrNode* node);
      virtual bool visit(CastNode* node);
      virtual bool visit(CondExprNode* node);
      virtual bool visit(FuncCallNode* node);
      virtual bool visit(ArrayRefNode* node);
      virtual bool visit(DereferenceNode* node);
      virtual bool visit(MemberRefNode* node);
      virtual bool visit(PtrMemberRefNode* node);
      virtual bool visit(VariableNode* node);
      virtual bool visit(IntegerLiteralNode* node);
      virtual bool visit(RealLiteralNode* node);
      virtual bool visit(StringLiteralNode* node);
      virtual bool visit(SizeofExprNode* node);
      virtual bool visit(SizeofTypeNode* node);
      virtual bool visit(UnaryOpNode* node);
      virtual bool visit(PrefixOpNode* node);   
      virtual bool visit(SuffixOpNode* node);

      // Etc
      virtual bool visit(FunctionDecl* node);
      virtual bool visit(VariableDecl* node);
      virtual bool visit(ConstantDecl* node);
      virtual bool visit(TypeNode* node); 
      virtual bool visit(ParamNode* node);
      virtual bool visit(ImportNode* node);
      virtual bool visit(ArgsNode* node);        
      virtual bool visit(ClassNode* node);   
      virtual bool visit(TypedefNode* node);  
  };
}

#endif
