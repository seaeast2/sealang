#ifndef _ast_node_h_
#define _ast_node_h_

#include <cstring>
#include "ast_type.h"


namespace AST {
// Abstrat Syntax Tree Base Nodes
  struct Location {
    std::string source_name_;
    int token_idx_;
  };

  class FunctionDecl;
  class ConstantDecl;
  class VariableDecl;
  class ParamNode;
  class StmtNode;
  class ExprNode;
  class CaseNode;

  typedef SimpleVector<FunctionDecl*> Functions;
  typedef SimpleVector<ConstantDecl*> Constants;
  typedef SimpleVector<VariableDecl*> Variables;
  typedef SimpleVector<ParamNode*> Params;
  typedef SimpleVector<StmtNode*> Stmts;
  typedef SimpleVector<ExprNode*> Exprs;
  typedef SimpleVector<CaseNode*> Cases;

  class BaseNode {
    public:
      enum NodeKind {
        BaseNodeTy,
          FunctionDeclTy,
          VariableDeclTy,
          ConstantDeclTy,
          TypeNodeTy,
          ParamNodeTy,
          StmtNodeTy,
            BlockNodeTy,
            LabelNodeTy,
            ExprStmtNodeTy,
            IfNodeTy,
            WhileNodeTy,
            DoWhileNodeTy,
            ForNodeTy,
            CaseNodeTy,
            SwitchNodeTy,
            GotoNodeTy,
            ReturnNodeTy,
            BreakNodeTy,
            ContinueNodeTy,
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

    public:
      BaseNode() {}
      virtual ~BaseNode() {}

      NodeKind GetNodeKind() { return kind_; }
      virtual bool IsKindOf(NodeKind kind) = 0;

      virtual void print();
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

  class ParamNode : public BaseNode {
    protected:
      bool var_arg_; // variable args
      Type* type_;
      std::string name_;
    public:
      ParamNode() {
        kind_ = ParamNodeTy;
        var_arg_ = false;
        type_ = nullptr;
      }
      ParamNode(AST::Type* ty, const char* name, bool var_arg) {
        kind_ = ParamNodeTy;
        var_arg_ = var_arg;
        type_ = ty;
        name_ = name;
      }
      virtual ~ParamNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ParamNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetVarArgs(bool var_arg) { var_arg_ = var_arg; }
      void SetType(Type* ty) { type_ = ty;  }
      void SetName(const char* name) { name_ = name; }

      bool GetVarArgs() { return var_arg_; }
      Type* GetType() { return type_; }
      const char* GetName() { return name_.c_str(); }
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

  class BlockNode : public StmtNode {
    protected:
      Variables vars_;
      Stmts     stmts_;

    public:
      BlockNode() {
        kind_ = BlockNodeTy;
      }

      virtual ~BlockNode();
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == BlockNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void AddVariable(VariableDecl* var) { vars_.PushBack(var); }
      void AddStmt(StmtNode* stmt) { stmts_.PushBack(stmt); }

      VariableDecl* GetVariable(int index);
      StmtNode* GetStmt(int index);

      int GetVarSize() { return vars_.GetSize(); }
      int GetStmtSize() { return stmts_.GetSize(); }

      void ReverseVariableOrder() { vars_.Reverse(); }
      void ReverseStmtOrder() { stmts_.Reverse(); }

  };

  class LabelNode : public StmtNode {
    protected:
      std::string    label_name_;
      StmtNode* stmt_;

    public:
      LabelNode() {
        kind_ = LabelNodeTy;
        stmt_ = nullptr;
      }

      LabelNode(const char* label_name, StmtNode* stmt) {
        kind_ = LabelNodeTy;
        label_name_ = label_name;
        stmt_ = stmt;
      }

      virtual ~LabelNode() {
        if (stmt_ != nullptr)
          delete stmt_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LabelNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }


      void SetLabelName(const char* label_name) {
        label_name_ = label_name;
      }
      const char* GetLabelName() { return label_name_.c_str(); }

      void SetStmt(StmtNode* stmt) { stmt_ = stmt; }
      StmtNode* GetStmt() { return stmt_; }
  };

  class ExprStmtNode : public StmtNode {
    protected:
      ExprNode* expr_;

    public:
      ExprStmtNode() {
        kind_ = ExprStmtNodeTy;
        expr_ = nullptr;
      }

      ExprStmtNode(ExprNode* expr) {
        kind_ = ExprStmtNodeTy;
        expr_ = expr;
      }

      virtual ~ExprStmtNode() {
        if (expr_ != nullptr)
          delete expr_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ExprStmtNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetExpr(ExprNode* expr) { expr_ = expr; }
      ExprNode* GetExpr() { return expr_; }
  };
  
  class IfNode : public StmtNode {
    protected:
      ExprNode* cond_;
      StmtNode* then_body_;
      StmtNode* else_body_;

    public:
      IfNode() {
        kind_ = IfNodeTy;
        cond_ = nullptr;
        then_body_ = nullptr;
        else_body_ = nullptr;
      }

      IfNode(ExprNode* cond, StmtNode* thenbody, StmtNode* elsebody) {
        kind_ = IfNodeTy;
        cond_ = cond;
        then_body_ = thenbody;
        else_body_ = elsebody;
      }

      virtual ~IfNode() {
        if (cond_)
          delete cond_;
        if (then_body_)
          delete then_body_;
        if (else_body_)
          delete else_body_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == IfNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetThenBody(StmtNode* thenbody) { then_body_ = thenbody; }
      void SetElseBody(StmtNode* elsebody) { else_body_ = elsebody; }

      ExprNode* GetCond() { return cond_; }
      StmtNode* GetThenBody() { return then_body_; }
      StmtNode* GetElseBody() { return else_body_; }
  };

  class WhileNode : public StmtNode {
    protected:
      ExprNode* cond_;
      StmtNode* body_;

    public:
      WhileNode() {
        kind_ = WhileNodeTy;
        cond_ = nullptr;
        body_ = nullptr;
      }

      WhileNode(ExprNode* cond, StmtNode* body) {
        kind_ = WhileNodeTy;
        cond_ = cond;
        body_ = body;
      }

      virtual ~WhileNode() {
        if (cond_)
          delete cond_;
        if (body_)
          delete body_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == WhileNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetBody(StmtNode* body) { body_ = body; }

      ExprNode* GetCond() { return cond_; }
      StmtNode* GetBody() { return body_; }
  };

  class DoWhileNode : public StmtNode {
    protected:
      ExprNode* cond_;
      StmtNode* body_;

    public:
      DoWhileNode() {
        kind_ = DoWhileNodeTy;
        cond_ = nullptr;
        body_ = nullptr;
      }

      DoWhileNode(StmtNode* body, ExprNode* cond) {
        kind_ = DoWhileNodeTy;
        cond_ = cond;
        body_ = body;
      }

      virtual ~DoWhileNode() {
        if (cond_)
          delete cond_;
        if (body_)
          delete body_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == DoWhileNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetBody(StmtNode* body) { body_ = body; }

      ExprNode* GetCond() { return cond_; }
      StmtNode* GetBody() { return body_; }
  };

  class ForNode : public StmtNode {
    protected:
      ExprNode* init_;
      ExprNode* cond_;
      ExprNode* inc_;
      StmtNode* body_;

    public:
      ForNode() {
        kind_ = ForNodeTy;
        init_ = nullptr;
        cond_ = nullptr;
        inc_ = nullptr;
        body_ = nullptr;
      }

      ForNode(ExprNode* init, ExprNode* cond, ExprNode* inc, StmtNode* body) {
        kind_ = ForNodeTy;
        init_ = init;
        cond_ = cond;
        inc_ = inc;
        body_ = body;
      }

      virtual ~ForNode() {
        if (init_) 
          delete init_;
        if (cond_)
          delete cond_;
        if (inc_)
          delete inc_;
        if (body_)
          delete body_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ForNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetInit(ExprNode* init) { init_ = init; }
      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetInc(ExprNode* inc) { inc_ = inc; }
      void SetBody(StmtNode* body) { body_ = body; }

      ExprNode* GetInit() { return init_; }
      ExprNode* GetCond() { return cond_; }
      ExprNode* GetInc() { return inc_; }
      StmtNode* GetBody() { return body_; }
  };

  class CaseNode : public StmtNode {
    protected:
      Exprs values_;
      StmtNode* body_;

    public:
      CaseNode() {
        kind_ = CaseNodeTy;
        body_ = nullptr;
      }

      CaseNode(Exprs* values, StmtNode* body) {
        kind_ = CaseNodeTy;
        values_ = *values;
        body_ = body;
      }

      virtual ~CaseNode() {
        if (!values_.IsEmpty()) {
          for (int i = 0; i < values_.GetSize(); i++) {
            delete values_[i];
          }
        }
        if (body_)
          delete body_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == CaseNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCase(ExprNode* value) { values_.PushBack(value); }
      void SetBody(StmtNode* body) { body_ = body; }

      int GetCaseValueNum() { return values_.GetSize(); }
      ExprNode* GetCaseValue(int index) { return values_[index]; }
      StmtNode* GetBody() { return body_; }

  };

  class SwitchNode : public StmtNode {
    protected:
      ExprNode* cond_; // case condition
      Cases cases_;

    public:
      SwitchNode() {
        kind_ = SwitchNodeTy;
        cond_ = nullptr;
      }

      virtual ~SwitchNode() {
        if (cond_)
          delete cond_;
        if (!cases_.IsEmpty()) {
          for (int i = 0; i < cases_.GetSize(); i++) {
            delete cases_[i];
          }
        }
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == SwitchNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCaseCond(ExprNode* cond) { cond_ = cond; }
      void SetCases(Cases* cases) { cases_ = cases; }

      ExprNode* GetCond() { return cond_; }
      int GetCaseNum() { return cases_.GetSize(); }
      CaseNode* GetCase(int index) { return cases_[index]; }
  };

  class AbstractAssignNode : public ExprNode {
    protected:
      ExprNode* lhs_, *rhs_;

    public:
      AbstractAssignNode() {
        kind_ = AbstractAssignNodeTy;
      }
      virtual ~AbstractAssignNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == AbstractAssignNodeTy|| kind == ExprNodeTy ||
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetLHS(ExprNode* lhs) { lhs_ = lhs; }
      void SetRHS(ExprNode* rhs) { rhs_ = rhs; }

      ExprNode* GetLHS() { return lhs_; }
      ExprNode* GetRHS() { return rhs_; }
  };

  class AssignNode : public AbstractAssignNode {
    public:
      AssignNode() {
        kind_ = AssignNodeTy;
      }
      AssignNode(ExprNode* lhs, ExprNode* rhs) {
        kind_ = AssignNodeTy;
        lhs_ = lhs;
        rhs_ = rhs;
      }
      virtual ~AssignNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == AssignNodeTy || kind == AbstractAssignNodeTy|| 
            kind == ExprNodeTy || kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class OpAssignNode : public AbstractAssignNode {
    public:
    enum AssignOp {
      AssignAdd,      // +=
      AssignSub,      // -=
      AssignMul,      // *=
      AssignDiv,      // /=
      AssignMod,      // %=
      AssignBitAnd,   // &=
      AssignBitOr,    // |=
      AssignBitXor,   // ^=
      AssignBitShiftL,// <<==
      AssignBitShiftR // >>==
    };

    protected:
    AssignOp op_;
    public:
      OpAssignNode() {
        kind_ = OpAssignNodeTy;
      }
      OpAssignNode(ExprNode* lhs, AssignOp op, ExprNode* rhs) {
        kind_ = OpAssignNodeTy;
        lhs_ = lhs;
        op_ = op;
        rhs_ = rhs;
      }
      virtual ~OpAssignNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == OpAssignNodeTy || kind == AbstractAssignNodeTy|| 
            kind == ExprNodeTy || kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetOp(AssignOp op) { op_ = op; }
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

  class BinaryOpNode : public ExprNode {
    public:
      enum BinOp {
        LogicOr,      // ||
        LogicAnd,     // &&
        GreatorThan,  // >
        LessThan,     // <
        GreatorThanEq,// >=
        LessThanEq,   // <=
        Equal,        // ==
        NotEqual,     // !=
        BitOr,        // |
        BitAnd,       // &
        BitXor,       // ^
        BitShiftLeft, // <<
        BitShiftRight,// >>
        BinSum,       // +
        BinSub,       // -
        BinMul,       // *
        BinDiv,       // /
        BinMod        // %
      };
    protected:
      BinOp   bin_op_;
      ExprNode *left_, *right_;
    public:
      BinaryOpNode() {
        kind_ = BinaryOpNodeTy;
      }
      BinaryOpNode(ExprNode* left, BinOp op, ExprNode* right) {
        kind_ = BinaryOpNodeTy;
        bin_op_ = op;
        left_ = left;
        right_ = right;
      }
      virtual ~BinaryOpNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == BinaryOpNodeTy || kind == ExprNodeTy ||
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetOp(BinOp op) { bin_op_ = op; }
      void SetLeft(ExprNode* left) { left_ = left; }
      void SetRight(ExprNode* right) { right_ = right; }

      BinOp     GetOp() { return bin_op_; }
      ExprNode* GetLeft() { return left_; }
      ExprNode* GetRight() { return right_; }
  };

  class LogicalAndNode : public BinaryOpNode {
    public:
      LogicalAndNode() {
        kind_ = LogicalAndNodeTy;

        bin_op_ = BinaryOpNode::LogicAnd;
      }
      LogicalAndNode(ExprNode* left, ExprNode* right) {
        kind_ = LogicalAndNodeTy;

        bin_op_ = BinaryOpNode::LogicAnd;
        left_ = left;
        right_ = right;
      }
      virtual ~LogicalAndNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LogicalAndNodeTy ||
            kind == BinaryOpNodeTy || kind == ExprNodeTy ||
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class LogicalOrNode : public BinaryOpNode {
    public:
      LogicalOrNode() {
        kind_ = LogicalOrNodeTy;
        bin_op_ = BinaryOpNode::LogicOr;
      }
      LogicalOrNode(ExprNode* left, ExprNode* right) {
        kind_ = LogicalOrNodeTy;
        bin_op_ = BinaryOpNode::LogicOr;
        left_ = left;
        right_ = right;
      }
      virtual ~LogicalOrNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LogicalOrNodeTy ||
            kind == BinaryOpNodeTy || kind == ExprNodeTy ||
            kind == StmtNodeTy || kind == BaseNodeTy)
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
        cast_type_ = nullptr;
        term_expr_ = nullptr;
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

  // therary operation.
  class CondExprNode : public ExprNode {
    ExprNode* cond_expr_, *then_expr_, *else_expr_;
    public:
      CondExprNode() {
        kind_ = CondExprNodeTy;
        cond_expr_ = then_expr_ = else_expr_ = nullptr;
      }
      CondExprNode(ExprNode* cond_expr, ExprNode* then_expr, ExprNode* else_expr) {
        kind_ = CondExprNodeTy;

        cond_expr_ = cond_expr;
        then_expr_ = then_expr;
        else_expr_ = else_expr;
      }
      virtual ~CondExprNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == CondExprNodeTy|| kind == ExprNodeTy ||
            kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond_expr) { cond_expr_ = cond_expr; }
      void SetThen(ExprNode* then_expr) { then_expr_ = then_expr; }
      void SetElse(ExprNode* else_expr) { else_expr_ = else_expr; }

      ExprNode* GetCond() { return cond_expr_; }
      ExprNode* GetThen() { return then_expr_; }
      ExprNode* GetElse() { return else_expr_; }
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
          return nullptr;
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
    std::string member_name_;
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
    std::string member_name_;
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




  class TypeDefinition : public BaseNode {
    protected:
    public:
      TypeDefinition() {
        kind_ = TypeDefinitionTy;
      }
      virtual ~TypeDefinition() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == TypeDefinitionTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class CompositeTypeDefinition : public TypeDefinition {
    protected:
    public:
      CompositeTypeDefinition() {
        kind_ = CompositeTypeDefinitionTy;
      }
      virtual ~CompositeTypeDefinition() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == CompositeTypeDefinitionTy|| kind == TypeDefinitionTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class FunctionDecl : public BaseNode {
    protected:
      bool is_static_; // storage
      Type* ret_ty_; // return type
      std::string name_; // function name
      Params params_; // parameters
      BlockNode* body_; // body

    public:
      FunctionDecl();
      FunctionDecl(bool storage, Type* retty, const char* fnname, Params* params, BlockNode* body); 
      virtual ~FunctionDecl();

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == FunctionDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) { is_static_ = st; }
      void SetReturnType(Type* ty) { ret_ty_ = ty; }
      void SetName(const char* fnname) { name_ = fnname; }
      void SetParams(Params* params);
      void SetBody(BlockNode* bd) { body_ = bd; }

      bool IsStatic() { return is_static_; }
      Type* GetReturnType() { return ret_ty_; }
      const char* GetName() { return name_.c_str(); }
      ParamNode* GetParamNode(unsigned int index);
      BlockNode* GetBody() { return body_; }

      //FunctionType* GetType(); // TODO: need to get funciton type
  };
  
  class VariableDecl : public BaseNode {
    bool is_static_;
    Type* type_;
    std::string name_;
    ExprNode* initializer_;

    public:
      VariableDecl() {
        kind_ = VariableDeclTy;
        is_static_ = false;
        type_ = nullptr;
        initializer_ = nullptr;
      }
      VariableDecl(Type* type, const char* name, ExprNode* init);
      virtual ~VariableDecl() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == VariableDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) {is_static_ = st;}
      void SetType(Type* type) { type_ = type; }
      void SetName(const char* name) { name_ = name; }
      void SetName(const char* name, int len);
      void SetInit(ExprNode* init) { initializer_ = init; }
  };

  class ConstantDecl : public BaseNode {
    Type* type_;
    std::string name_;
    ExprNode* initializer_;

    public:
      ConstantDecl() {
        kind_ = ConstantDeclTy;
        type_ = nullptr;
        initializer_ = nullptr;
      }
      ConstantDecl(Type* type, const char* name, ExprNode* init);
      virtual ~ConstantDecl() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ConstantDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetType(Type* type) { type_ = type; }
      void SetName(const char* name) { name_ = name; }
      void SetName(const char* name, int len);
      void SetInit(ExprNode* init) { initializer_ = init; }
  };

  class ClassNode : public CompositeTypeDefinition {
    protected:
      Variables member_variables_;
      Functions member_functions_;
    public:
      ClassNode() {
        kind_ = ClassNodeTy;
      }
      ClassNode(Variables* mem_var, Functions* mem_func);
      virtual ~ClassNode();

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ClassNodeTy || kind == CompositeTypeDefinitionTy ||
            kind == TypeDefinitionTy|| kind == BaseNodeTy)
          return true;
        return false;
      }

      void AddMemVariable(VariableDecl* var) { member_variables_.PushBack(var); }
      void AddMemFunction(FunctionDecl* fun) { member_functions_.PushBack(fun); }

      VariableDecl* GetMemVariable(unsigned int index);
      FunctionDecl* GetMemFunction(unsigned int index);

      int GetMemVarSize() { return member_variables_.GetSize(); }
      int GetMemFunSize() { return member_functions_.GetSize(); }

      void ReverseVariableOrder() { member_variables_.Reverse(); }
      void ReverseFunctionOrder() { member_functions_.Reverse(); }
  };

  class Declarations {
    private:
      Functions funcs_;
      Constants conss_;
      Variables vars_;

    public:
      Declarations() {}
      ~Declarations() {}
  };


};
#endif
