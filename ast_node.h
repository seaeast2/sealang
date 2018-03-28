#ifndef _ast_node_h_
#define _ast_node_h_

#include <string.h>
#include "ast_type.h"

using namespace std;

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

  class LHSNode : public ExprNode {
    public:
      LHSNode() {
        kind_ = LHSNodeTy;
      }
      virtual ~LHSNode() {}
  };

  class VariableNode : public LHSNode {
    char* name_;
    int str_len_;
    public:
      VariableNode() {
        kind_ = VariableNodeTy;
      }
      VariableNode(const char* name, int str_len) {
        kind_ = VariableNodeTy;
        name_ = new char[str_len];
        strcpy(name_, name);
        str_len_ = str_len;
      }
      virtual ~VariableNode() {}
  };

  class UnaryOpNode : public ExprNode {
    public:
    enum UnaryOp {
      Pos, // + 
      Neg, // -
      Inc, // ++
      Dec, // --
      Not, // !
      BitNot, // ~ : bit not
      DeRef, // * : pointer dereference
      Addr // & : address operator
    };
    protected:
    ExprNode * expr_;
    UnaryOp op_;

    public:
      UnaryOpNode() {
        kind_ = UnaryOpNodeTy;
      }
      virtual ~UnaryOpNode() {}
      ExprNode* GetExpr() { return expr_; }
  };

  class UnaryArithmeticOpNode : public UnaryOpNode{
    public:
      UnaryArithmeticOpNode() {
        kind_ = UnaryArithmeticOpNodeTy;
      }
      virtual ~UnaryArithmeticOpNode() {}
  };

  class PrefixOpNode : public UnaryArithmeticOpNode{
    public:
      PrefixOpNode() {
        kind_ = PrefixOpNodeTy;
      }
      virtual ~PrefixOpNode() {}
  };

  class SuffixOpNode : public UnaryArithmeticOpNode{
    public:
      SuffixOpNode(ExprNode *expr, UnaryOp op) {
        kind_ = SuffixOpNodeTy;
        expr_ = expr; 
        op_ = op;
      }
      virtual ~SuffixOpNode() {}
  };

  class LiteralNode : public ExprNode {
    public:
      LiteralNode() {
        kind_ = LiteralNodeTy;
      }
      virtual ~LiteralNode() {}
  };

  class IntegerLiteralNode : public LiteralNode {
    public:
    enum IntLitralKind {
      Int,
      Char
    };

    private:
    long value_;
    IntLitralKind literal_type_;
    
    public:
      IntegerLiteralNode() { kind_ = IntegerLiteralNodeTy; }
      IntegerLiteralNode(IntLitralKind ltype, long value) { kind_ = IntegerLiteralNodeTy; 
        value_ = value; 
        literal_type_ = ltype;
      }
      virtual ~IntegerLiteralNode() {}
      long GetValue() { return value_; }
  };

  class StringLiteralNode : public LiteralNode {
    private:
      char* str_;
      int len_;
    
    public:
      StringLiteralNode() { kind_ = StringLiteralNodeTy; }
      StringLiteralNode(const char* str, int str_len) { kind_ = StringLiteralNodeTy; 
        str_ = new char[str_len];
        strcpy(str_, str); // Need to check string length
        len_ = str_len;
      }
      virtual ~StringLiteralNode() {}
      const char* GetStr() { return str_; }
  };

};
#endif
