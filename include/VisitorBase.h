#ifndef _visitor_base_h_
#define _visitor_base_h_

#include "Common.h"

namespace AST {
  class VisitorBase {
    public:
      VisitorBase() {}
      virtual ~VisitorBase() {}

      virtual bool Visit(BaseNode* node)        {return true;} 

      // Statements
      virtual bool Visit(BlockNode* node)        {return true;} 
      virtual bool Visit(LabelNode* node)        {return true;}
      virtual bool Visit(ExprStmtNode* node)     {return true;}
      virtual bool Visit(IfNode* node)           {return true;}
      virtual bool Visit(WhileNode* node)        {return true;}
      virtual bool Visit(DoWhileNode* node)      {return true;}
      virtual bool Visit(ForNode* node)          {return true;}
      virtual bool Visit(CaseNode* node)         {return true;}
      virtual bool Visit(SwitchNode* node)       {return true;}
      virtual bool Visit(BreakNode* node)        {return true;}
      virtual bool Visit(ContinueNode* node)     {return true;}
      virtual bool Visit(GotoNode* node)         {return true;}
      virtual bool Visit(ReturnNode* node)       {return true;}

      // Expression
      virtual bool Visit(AssignNode* node)       {return true;}
      virtual bool Visit(OpAssignNode* node)     {return true;}
      virtual bool Visit(AddressNode* node)      {return true;}
      virtual bool Visit(BinaryOpNode* node)     {return true;}
      virtual bool Visit(LogicalAndNode* node)   {return true;}
      virtual bool Visit(LogicalOrNode* node)    {return true;}
      virtual bool Visit(CastNode* node)         {return true;}
      virtual bool Visit(CondExprNode* node)     {return true;}
      virtual bool Visit(FuncCallNode* node)     {return true;}
      virtual bool Visit(ArrayRefNode* node)     {return true;}
      virtual bool Visit(DereferenceNode* node)  {return true;}
      virtual bool Visit(MemberRefNode* node)    {return true;}
      virtual bool Visit(PtrMemberRefNode* node) {return true;}
      virtual bool Visit(VariableNode* node)     {return true;}
      virtual bool Visit(IntegerLiteralNode* node){return true;}
      virtual bool Visit(RealLiteralNode* node)  {return true;}
      virtual bool Visit(StringLiteralNode* node){return true;}
      virtual bool Visit(SizeofExprNode* node)   {return true;}
      virtual bool Visit(SizeofTypeNode* node)   {return true;}
      virtual bool Visit(UnaryOpNode* node)      {return true;}
      virtual bool Visit(PrefixOpNode* node)     {return true;}
      virtual bool Visit(SuffixOpNode* node)     {return true;}

      // Etc
      virtual bool Visit(FunctionDecl* node)     {return true;}
      virtual bool Visit(VariableDecl* node)     {return true;}
      virtual bool Visit(ConstantDecl* node)     {return true;}
      virtual bool Visit(TypeNode* node)         {return true;}
      virtual bool Visit(ParamNode* node)        {return true;}
      virtual bool Visit(ImportNode* node)       {return true;}
      virtual bool Visit(ArgsNode* node)         {return true;}
      virtual bool Visit(ClassNode* node)        {return true;}
      virtual bool Visit(TypedefNode* node)      {return true;}
  };
}

#endif
