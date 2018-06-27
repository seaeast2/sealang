#ifndef _ast_visitor_h_
#define _ast_visitor_h_

namespace AST {

  class BlockNode;
  class LabelNode;
  class ExprStmtNode;
  class IfNode;
  class WhileNode;
  class DoWhileNode;
  class ForNode;
  class CaseNode;
  class SwitchNode;
  class BreakNode;
  class ContinueNode;
  class GotoNode;
  class ReturnNode;
  class AssignNode;
  class OpAssignNode;
  class AddressNode;
  class BinaryOpNode;
  class LogicalAndNode;
  class LogicalOrNode;
  class CastNode;
  class CondExprNode;
  class FuncCallNode;
  class ArrayRefNode;
  class DereferenceNode;
  class MemberRefNode;
  class PtrMemberRefNode;
  class VariableNode;
  class IntegerLiteralNode;
  class RealLiteralNode;
  class StringLiteralNode;
  class SizeofExprNode;
  class SizeofTypeNode;
  class UnaryOpNode;
  class PrefixOpNode;
  class SuffixOpNode;

  template<class E> 
  class VisitorBase {
    public:
      VisitorBase() {}
      virtual ~VisitorBase() {}

      // Statements
      virtual E visit(BlockNode* node)        {return E();} 
      virtual E visit(LabelNode* node)        {return E();}
      virtual E visit(ExprStmtNode* node)     {return E();}
      virtual E visit(IfNode* node)           {return E();}
      virtual E visit(WhileNode* node)        {return E();}
      virtual E visit(DoWhileNode* node)      {return E();}
      virtual E visit(ForNode* node)          {return E();}
      virtual E visit(CaseNode* node)         {return E();}
      virtual E visit(SwitchNode* node)       {return E();}
      virtual E visit(BreakNode* node)        {return E();}
      virtual E visit(ContinueNode* node)     {return E();}
      virtual E visit(GotoNode* node)         {return E();}
      virtual E visit(ReturnNode* node)       {return E();}

      // Expression
      virtual E visit(AssignNode* node)       {return E();}
      virtual E visit(OpAssignNode* node)     {return E();}
      virtual E visit(AddressNode* node)      {return E();}
      virtual E visit(BinaryOpNode* node)     {return E();}
      virtual E visit(LogicalAndNode* node)   {return E();}
      virtual E visit(LogicalOrNode* node)    {return E();}
      virtual E visit(CastNode* node)         {return E();}
      virtual E visit(CondExprNode* node)     {return E();}
      virtual E visit(FuncCallNode* node)     {return E();}
      virtual E visit(ArrayRefNode* node)     {return E();}
      virtual E visit(DereferenceNode* node)  {return E();}
      virtual E visit(MemberRefNode* node)    {return E();}
      virtual E visit(PtrMemberRefNode* node) {return E();}
      virtual E visit(VariableNode* node)     {return E();}
      virtual E visit(IntegerLiteralNode* node){return E();}
      virtual E visit(RealLiteralNode* node)  {return E();}
      virtual E visit(StringLiteralNode* node){return E();}
      virtual E visit(SizeofExprNode* node)   {return E();}
      virtual E visit(SizeofTypeNode* node)   {return E();}
      virtual E visit(UnaryOpNode* node)      {return E();}
      virtual E visit(PrefixOpNode* node)     {return E();}
      virtual E visit(SuffixOpNode* node)     {return E();}
  };

}


#endif
