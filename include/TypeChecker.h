#ifndef _type_checker_h_
#define _type_checker_h_

#include "ASTNode.h"
#include "ASTVisitor.h"

namespace AST {
  class TypeChecker : public ASTVisitor {
    private:
      // Check 
      bool CheckInvalidVariable(VariableDecl* VD);

      // 1. check void type record type, array
      //   ex) void a; // void variable
      //   ex) void[10] b; // void array
      //   ex) class { void c; } // void member
      bool CheckInvalidVariableType(Type* t);
      // 2. check Duplicated member variable
      // ex ) class { int aa; short aa};
      bool CheckDuplicatedMemberName(RecordDecl* r);

      // TODO : Need to add ErrorHandler

    public:
      TypeChecker();
      virtual ~TypeChecker();

      bool Check(Declarations* decls);

      
      // Statements
      /*virtual bool Visit(BlockNode* node) override;
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
      virtual bool Visit(FunctionDecl* node) override;*/
      virtual bool Visit(VariableDecl* node) override;
      /*virtual bool Visit(ConstantDecl* node) override;
      virtual bool Visit(TypeNode* node) override; */
      virtual bool Visit(ParamDecl* node) override;
      /*virtual bool Visit(ImportNode* node) override;
      virtual bool Visit(ArgsNode* node) override;*/
      virtual bool Visit(RecordDecl* node) override;   
      //virtual bool Visit(TypedefNode* node) override;  
  };
};

#endif
