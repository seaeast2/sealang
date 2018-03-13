#ifndef _ast_node_h_
#define _ast_node_h_

#include "ast_type.h"


namespace AST {
// Abstrat Syntax Tree Base Nodes
  struct Location {
    string source_name_;
    int token_idx_;
  };

  class BaseNode {
    public:
      enum NodeKind {
        BaseNodeTy,
        RootNodeTy,
        ExprNodeTy,
          AbstractAssignNodeTy,
            AssignNodeTy,
            OpAssignNodeTy,
          AddressNodeTy,
          BinaryOpNodeTy,
            LogicalAndNodeTy,
            LogicalOrNodeTy,
          CastNodeTy,
          CondExprNodeTy,
          FuncCallNodeTy,
          LHSNodeTy,
            ArrayRefNodeTy,
            DereferenceNodeTy,
            MemberRefNodeTy,
            PtrMemberRefNodeTy,
            VariableNodeTy,
          LiteralNodeTy,
            IntegerLiteralNodeTy,
            RealLiteralNodeTy,
            StringLiteralNodeTy,
          SizeofExprNodeTy,
          SizeofTypeNodeTy,
          UnaryOpNodeTy,
            UnaryArithmeticOpNodeTy,
              PrefixOpNodeTy,
              SuffixOpNodeTy,
          MemberNodeTy,
          StmtNodeTy,
            BlockNodeTy,
            BreakNodeTy,
            CaseNodeTy,
            ContinueNodeTy,
            DoWhileNodeTy,
            ExprStmtNodeTy,
            ForNodeTy,
            GotoNodeTy,
            IfNodeTy,
            LabelNodeTy,
            ReturnNodeTy,
            SwitchNodeTy,
            WhileNodeTy,
        TypeDefinitionTy,
          CompositeTypeDefinitionTy,
            ClassNodeTy,
          TypedefNodeTy,
        TypeNodeTy
      };
    protected:
      NodeKind kind_;
      Location loc_;
      AST::Type type_;

    public:
      BaseNode() {}
      virtual ~BaseNode() {}

      NodeKind GetNodeKind() { return kind_; }

      virtual void print();
  };


  class RootNode : public BaseNode {
    protected:

    public:
      RootNode() {}
      virtual ~RootNode() {}
  };

  class ExprNode : public BaseNode {
    public:
      ExprNode() {}
      virtual ~ExprNode() {}
  };

  class LiteralNode : public ExprNode {
    public:
      LiteralNode() {}
      virtual ~LiteralNode() {}
  };

  class IntegerLiteralNode : public LiteralNode {
    long value_;
    
    public:
      IntegerLiteralNode() { kind_ = IntegerLiteralNodeTy; }
      IntegerLiteralNode(long value) { kind_ = IntegerLiteralNodeTy; value_ = value; }
      ~IntegerLiteralNode() {}
      long GetValue() { return value_; }
  };

};
#endif
