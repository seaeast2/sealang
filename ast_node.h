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
          FunctionNodeTy,
          ParamsNodeTy, // function parameters
          TypeNodeTy,
          StmtNodeTy,
            BlockNodeTy,
            BreakNodeTy,
            CaseNodeTy,
            ContinueNodeTy,
            DoWhileNodeTy,
            ExprStmtNodeTy, // ??
            ForNodeTy,
            GotoNodeTy,
            IfNodeTy,
            LabelNodeTy,
            ReturnNodeTy,
            SwitchNodeTy,
            WhileNodeTy,
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
              MemberNodeTy, // struct or class member
              ArgsNodeTy, // function call arguments
          TypeDefinitionTy,
            CompositeTypeDefinitionTy,
              ClassNodeTy,
            TypedefNodeTy,
      };
    protected:
      NodeKind kind_;
      Location loc_;
      AST::Type type_;

    public:
      BaseNode() {}
      virtual ~BaseNode() {}

      NodeKind GetNodeKind() { return kind_; }
      virtual bool IsKindOf(NodeKind kind) = 0;

      virtual void print();
  };


  class RootNode : public BaseNode {
    protected:

    public:
      RootNode() {
        kind_ = RootNodeTy;
      }
      virtual ~RootNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == RootNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class TypeNode : public BaseNode {
    AST::Type* type_;
    public:
      TypeNode() {
        kind_ = TypeNodeTy;
      }
      TypeNode(AST::Type* ty) {
        kind_ = TypeNodeTy;
        type_ = ty;
      }
      virtual ~TypeNode() {}

      AST::Type* GetType() { 
        return type_;
      }

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == TypeNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class StmtNode : public BaseNode {
    protected:

    public:
      StmtNode() {
        kind_ = StmtNodeTy;
      }
      virtual ~StmtNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ExprNode : public StmtNode {
    public:
      ExprNode() {
        kind_ = ExprNodeTy;
      }
      virtual ~ExprNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ExprNodeTy|| kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class AddressNode : public ExprNode {
    ExprNode * expr_;
    public:
      AddressNode() {
        kind_ = AddressNodeTy;
      }
      AddressNode(ExprNode* expr) {
        kind_ = AddressNodeTy;
        expr_ = expr;
      }
      virtual ~AddressNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == AddressNodeTy || 
            kind == ExprNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class CastNode : public ExprNode {
    TypeNode * cast_type_;
    ExprNode * term_expr_;
    public:
      CastNode() {
        kind_ = CastNodeTy;
        cast_type_ = NULL;
        term_expr_ = NULL;
      }

      CastNode(ExprNode* term, TypeNode* castty) {
        kind_ = CastNodeTy;
        term_expr_ = term;
        cast_type_ = castty;
      }

      virtual ~CastNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == CastNodeTy || 
            kind == ExprNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ArgsNode : public ExprNode {
    enum {MAX_ARGS = 30};

    ExprNode* args_[MAX_ARGS];
    int count_;
    public:
      ArgsNode() {
        kind_ = ArgsNodeTy;
        count_ = 0;
      }
      virtual ~ArgsNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ArgsNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      int Add(ExprNode* arg) {
        if (count_ == MAX_ARGS)
          return -1;

        args_[count_++] = arg;
        return count_;
      }

      int GetCount() {
        return count_;
      }

      ExprNode* GetArg(int idx) {
        if (idx < 0 || idx >= count_)
          return NULL;
        return args_[idx];
      }
  };

  class FuncCallNode : public ExprNode {
    ExprNode* func_expr_;
    ArgsNode* args_;
    public:
      FuncCallNode() {
        kind_ = FuncCallNodeTy;
      }

      FuncCallNode(ExprNode* func, ArgsNode* args) {
        kind_ = FuncCallNodeTy;
        func_expr_ = func;
        args_ = args;
      }
      virtual ~FuncCallNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == FuncCallNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class LHSNode : public ExprNode {
    public:
      LHSNode() {
        kind_ = LHSNodeTy;
      }
      virtual ~LHSNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ArrayRefNode : public LHSNode {
    // TODO : need to add base type
    ExprNode* expr_; // array varibale expr
    ExprNode* array_size_expr_;
    int array_size_;
    public:
      ArrayRefNode() {
        kind_ = ArrayRefNodeTy;
        array_size_ = 0;
      }
      ArrayRefNode(ExprNode* expr, ExprNode* arr_size) {
        kind_ = ArrayRefNodeTy;
        array_size_ = 0;
        array_size_expr_ = arr_size;
        expr_ = expr;
      }
      virtual ~ArrayRefNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ArrayRefNodeTy ||
            kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class DereferenceNode : public LHSNode {
    ExprNode* expr_;
    public:
      DereferenceNode() {
        kind_ = DereferenceNodeTy;
      }
      DereferenceNode(ExprNode* expr) {
        kind_ = DereferenceNodeTy;
        expr_ = expr;
      }
      virtual ~DereferenceNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == DereferenceNodeTy || 
            kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class MemberRefNode : public LHSNode {
    ExprNode* expr_;
    string member_name_;
    public:
      MemberRefNode() {
        kind_ = MemberRefNodeTy;
      }
      MemberRefNode(ExprNode* expr, const char* mbname) { 
        kind_ = MemberRefNodeTy;
        expr_ = expr;
        member_name_ = mbname;
      }
      virtual ~MemberRefNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == MemberRefNodeTy ||
            kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class PtrMemberRefNode : public LHSNode {
    ExprNode* expr_;
    string member_name_;
    public:
      PtrMemberRefNode() {
        kind_ = PtrMemberRefNodeTy;
      }
      PtrMemberRefNode(ExprNode* expr, const char* mbname) { 
        kind_ = PtrMemberRefNodeTy;
        expr_ = expr;
        member_name_ = mbname;
      }
      virtual ~PtrMemberRefNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == PtrMemberRefNodeTy ||
            kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
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

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == VariableNodeTy ||
            kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class SizeofExprNode : public ExprNode {
    ExprNode* expr_;
    public:
      SizeofExprNode() {
        kind_ = SizeofExprNodeTy;
      }
      SizeofExprNode(ExprNode* expr) {
        kind_ = SizeofExprNodeTy;
        expr_ = expr;
      }
      virtual ~SizeofExprNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == SizeofExprNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class SizeofTypeNode : public ExprNode {
    ExprNode * expr_;
    public:
      SizeofTypeNode() {
        kind_ = SizeofTypeNodeTy;
      }
      SizeofTypeNode(ExprNode* expr) {
        kind_ = SizeofTypeNodeTy;
        expr_ = expr;
      }
      virtual ~SizeofTypeNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == SizeofTypeNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
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
      UnaryOpNode(ExprNode* expr, UnaryOpNode::UnaryOp op) {
        kind_ = UnaryOpNodeTy;
        expr_ = expr;
        op_ = op;
      }
      virtual ~UnaryOpNode() {}
      ExprNode* GetExpr() { return expr_; }

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class UnaryArithmeticOpNode : public UnaryOpNode{
    public:
      UnaryArithmeticOpNode() {
        kind_ = UnaryArithmeticOpNodeTy;
      }
      virtual ~UnaryArithmeticOpNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class PrefixOpNode : public UnaryArithmeticOpNode{
    public:
      PrefixOpNode() {
        kind_ = PrefixOpNodeTy;
      }
      PrefixOpNode(ExprNode *expr, UnaryOp op) {
        kind_ = PrefixOpNodeTy;
        expr_ = expr; 
        op_ = op;
      }
      virtual ~PrefixOpNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == PrefixOpNodeTy ||
            kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class SuffixOpNode : public UnaryArithmeticOpNode{
    public:
      SuffixOpNode() {
        kind_ = SuffixOpNodeTy;
      }
      SuffixOpNode(ExprNode *expr, UnaryOp op) {
        kind_ = SuffixOpNodeTy;
        expr_ = expr; 
        op_ = op;
      }
      virtual ~SuffixOpNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == SuffixOpNodeTy ||
            kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class LiteralNode : public ExprNode {
    public:
      LiteralNode() {
        kind_ = LiteralNodeTy;
      }
      virtual ~LiteralNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
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

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == IntegerLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
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

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == StringLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };


};
#endif
