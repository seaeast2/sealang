#ifndef _type_checker_h_
#define _type_checker_h_

#include "ASTNode.h"
#include "ASTVisitor.h"

namespace AST {
  /* Check below error
   * 1. Assign RValue : 1 = 2 + 3
   * 2. Invalid Function Call. Check function pointer : "string"("%d\n", i)
   * 3. Invalid Array reference : 1[0]
   * 4. Invalid member reference : 10.memb
   * 5. Invalid member pointer reference : 10->memb
   * 6. Invalid pointer reference : *1
   * 7. Invalid RValue reference : &1
   * 8. Invalid RValue increase : ++1, 1++
   */
  class TypeChecker : public ASTVisitor {
    private:

    public:
      TypeChecker();
      virtual ~TypeChecker();

      /*
      // Statements
      virtual bool Visit(BlockNode* node) override;
      virtual bool Visit(LabelNode* node) override;
      virtual bool Visit(ExprStmtNode* node) override;
      virtual bool Visit(IfNode* node) override;    
      virtual bool Visit(WhileNode* node) override;
      virtual bool Visit(DoWhileNode* node) override;
      virtual bool Visit(ForNode* node) override;
      virtual bool Visit(CaseNode* node) override;
      virtual bool Visit(SwitchNode* node) override;
      virtual bool Visit(BreakNode* node) override;
      virtual bool Visit(ContinueNode* node) override;
      virtual bool Visit(GotoNode* node) override;
      virtual bool Visit(ReturnNode* node) override;

      // Expression
      virtual bool Visit(AssignNode* node) override;
      virtual bool Visit(OpAssignNode* node) override;
      virtual bool Visit(AddressNode* node) override;
      virtual bool Visit(BinaryOpNode* node) override;
      virtual bool Visit(LogicalAndNode* node) override;
      virtual bool Visit(LogicalOrNode* node) override;
      virtual bool Visit(CastNode* node) override;
      virtual bool Visit(CondExprNode* node) override;
      virtual bool Visit(FuncCallNode* node) override;
      virtual bool Visit(ArrayRefNode* node) override;
      virtual bool Visit(DereferenceNode* node) override;
      virtual bool Visit(MemberRefNode* node) override;
      virtual bool Visit(PtrMemberRefNode* node) override;
      virtual bool Visit(VariableNode* node) override;
      virtual bool Visit(IntegerLiteralNode* node) override;
      virtual bool Visit(RealLiteralNode* node) override;
      virtual bool Visit(StringLiteralNode* node) override;
      virtual bool Visit(SizeofExprNode* node) override;
      virtual bool Visit(SizeofTypeNode* node) override;
      virtual bool Visit(UnaryOpNode* node) override;
      virtual bool Visit(PrefixOpNode* node) override;   
      virtual bool Visit(SuffixOpNode* node) override;

      // Etc
      virtual bool Visit(FunctionDecl* node) override;
      virtual bool Visit(VariableDecl* node) override;
      virtual bool Visit(ConstantDecl* node) override;
      virtual bool Visit(TypeNode* node) override; 
      virtual bool Visit(ParamDecl* node) override;
      virtual bool Visit(ImportNode* node) override;
      virtual bool Visit(ArgsNode* node) override;        
      virtual bool Visit(ClassNode* node) override;   
      virtual bool Visit(TypedefNode* node) override;  
      */
  };
};

#endif
