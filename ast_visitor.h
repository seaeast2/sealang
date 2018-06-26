#ifndef _ast_visitor_h_
#define _ast_visitor_h_

#include "ast_node.h"

namespace AST {

  template<class T> T dummy_f() { T t; return t; }
  template<> void dummy_f<void>() { return; }
  
  template<class E, class S> 
  class VisitorBase {
    public:
      VisitorBase() {}
      virtual ~VisitorBase() {}

      // Statements
      virtual S visit(BlockNode* node) { return dummy_f<S>(); }
      virtual S visit(LabelNode* node) { return dummy_f<S>(); }
      virtual S visit(ExprStmtNode* node) { return dummy_f<S>(); }
      virtual S visit(IfNode* node) { return dummy_f<S>(); };
      virtual S visit(WhileNode* node) { return dummy_f<S>(); }
      virtual S visit(DoWhileNode* node) { return dummy_f<S>(); }
      virtual S visit(ForNode* node) { return dummy_f<S>(); }
      virtual S visit(CaseNode* node) { return dummy_f<S>(); }
      virtual S visit(SwitchNode* node) { return dummy_f<S>(); }
      virtual S visit(BreakNode* node) { return dummy_f<S>(); }
      virtual S visit(ContinueNode* node) { return dummy_f<S>(); }
      virtual S visit(GotoNode* node) { return dummy_f<S>(); }
      virtual S visit(ReturnNode* node) { return dummy_f<S>(); }

      // Expression
      virtual E visit(AssignNode* node) { return dummy_f<E>(); }
      virtual E visit(OpAssignNode* node) { return dummy_f<E>(); }
      virtual E visit(AddressNode* node) { return dummy_f<E>(); }
      virtual E visit(BinaryOpNode* node) { return dummy_f<E>(); }
      virtual E visit(LogicalAndNode* node) { return dummy_f<E>(); }
      virtual E visit(LogicalOrNode* node) { return dummy_f<E>(); }
      virtual E visit(CastNode* node) { return dummy_f<E>(); }
      virtual E visit(CondExprNode* node) { return dummy_f<E>(); }
      virtual E visit(FuncCallNode* node) { return dummy_f<E>(); }
      virtual E visit(ArrayRefNode* node) { return dummy_f<E>(); }
      virtual E visit(DereferenceNode* node) { return dummy_f<E>(); }
      virtual E visit(MemberRefNode* node) { return dummy_f<E>(); }
      virtual E visit(PtrMemberRefNode* node) { return dummy_f<E>(); }
      virtual E visit(VariableNode* node) { return dummy_f<E>(); }
      virtual E visit(IntegerLiteralNode* node) { return dummy_f<E>(); }
      virtual E visit(RealLiteralNode* node) { return dummy_f<E>(); }
      virtual E visit(StringLiteralNode* node) { return dummy_f<E>(); }
      virtual E visit(SizeofExprNode* node) { return dummy_f<E>(); }
      virtual E visit(SizeofTypeNode* node) { return dummy_f<E>(); }
      virtual E visit(UnaryOpNode* node) { return dummy_f<E>(); }
      virtual E visit(PrefixOpNode* node) { return dummy_f<E>(); }
      virtual E visit(SuffixOpNode* node) { return dummy_f<E>(); }
  };
}


#endif
