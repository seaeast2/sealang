#ifndef _ast_node_h_
#define _ast_node_h_

#include <cstring>
#include "ast_type.h"
#include "common.h"
#include "visitor_base.h"


namespace AST {
// Abstrat Syntax Tree Base Nodes
  struct Location {
    std::string source_name_;
    int token_idx_;
  };


  class BaseNode {
    public:
      enum NodeKind {
        BaseNodeTy,
          FunctionDeclTy,
          VariableDeclTy,
          ConstantDeclTy,
          TypeNodeTy,
          ParamNodeTy,
          ImportNodeTy,
          ArgsNodeTy, // function call arguments

          // Statements
          StmtNodeTy,
            BlockNodeTy,// visitor
            LabelNodeTy,// visitor
            ExprStmtNodeTy,// visitor
            IfNodeTy,// visitor
            WhileNodeTy,// visitor
            DoWhileNodeTy,// visitor
            ForNodeTy,// visitor
            CaseNodeTy,// visitor
            SwitchNodeTy,// visitor
            BreakNodeTy,// visitor
            ContinueNodeTy,// visitor
            GotoNodeTy,// visitor
            ReturnNodeTy,// visitor

          // Expressions
          ExprNodeTy,
            AbstractAssignNodeTy,
              AssignNodeTy,// visitor
              OpAssignNodeTy,// visitor
            AddressNodeTy, // visitor          
            BinaryOpNodeTy,// visitor
              LogicalAndNodeTy,// visitor
              LogicalOrNodeTy,// visitor
            CastNodeTy,// visitor
            CondExprNodeTy,// visitor
            FuncCallNodeTy,// visitor
            LHSNodeTy,
              ArrayRefNodeTy,// visitor
              DereferenceNodeTy,// visitor
              MemberRefNodeTy,// visitor
              PtrMemberRefNodeTy,// visitor
              VariableNodeTy,// visitor
            LiteralNodeTy,
              IntegerLiteralNodeTy,// visitor
              RealLiteralNodeTy, // visitor
              StringLiteralNodeTy,// visitor
            SizeofExprNodeTy,// visitor
            SizeofTypeNodeTy,// visitor
            UnaryOpNodeTy,// visitor
              UnaryArithmeticOpNodeTy,
                PrefixOpNodeTy,// visitor
                SuffixOpNodeTy,// visitor

          // User defined types
          TypeDefinitionTy,
            CompositeTypeDefinitionTy,
              ClassNodeTy,
            TypedefNodeTy,
      };
    protected:
      NodeKind kind_;
      Location loc_;

      BaseNode() { kind_ = BaseNodeTy; }
    public:
      virtual ~BaseNode() {}

      NodeKind GetNodeKind() { return kind_; }
      virtual bool IsKindOf(NodeKind kind) = 0;

      virtual bool Accept(VisitorBase* visitor) {
        return visitor->Visit(this);
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
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == TypeNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetType(Type* ty) { type_ = ty; }
      AST::Type* GetType() { return type_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ParamNode : public BaseNode {
    protected:
      bool var_arg_; // variable args
      TypeNode* type_;
      std::string name_;
    public:
      ParamNode() {
        kind_ = ParamNodeTy;
        var_arg_ = false;
        type_ = nullptr;
      }
      ParamNode(TypeNode* ty, const char* name, bool var_arg) {
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
      void SetType(TypeNode* ty) { type_ = ty;  }
      void SetName(const char* name) { name_ = name; }

      bool GetVarArgs() { return var_arg_; }
      TypeNode* GetType() { return type_; }
      const char* GetName() { return name_.c_str(); }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ImportNode : public BaseNode {
    protected:
      SimpleVector<std::string> import_paths_;
      char* complete_path_;

    public:
      ImportNode() {
        kind_ = ImportNodeTy;
        complete_path_ = nullptr;
      }
      virtual ~ImportNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ImportNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void AddImportPath(const char* path) { import_paths_.PushBack(path); }
      const char* GetImportPath();
      void Reverse() { import_paths_.Reverse(); }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class StmtNode : public BaseNode {
    protected:
      StmtNode() {
        kind_ = StmtNodeTy;
      }
    public:
      virtual ~StmtNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == StmtNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ExprNode : public BaseNode {
    protected:
      ExprNode() {
        kind_ = ExprNodeTy;
      }
    public:
      virtual ~ExprNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ExprNodeTy|| 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class BlockNode : public StmtNode {
    protected:
      VariableDecls vars_;
      StmtNodes     stmts_;

    public:
      BlockNode() {
        kind_ = BlockNodeTy;
      }

      BlockNode(VariableDecls* vars, StmtNodes* stmts) {
        kind_ = BlockNodeTy;
        if (vars)
          vars_ = *vars;
        if (stmts)
          stmts_ = *stmts;
      }

      virtual ~BlockNode();
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == BlockNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetVariables(VariableDecls* vars) { if (vars) vars_ = *vars; }
      void AddVariable(VariableDecl* var) { vars_.PushBack(var); }
      void SetStmts(StmtNodes* stmts) { if (stmts) stmts_ = *stmts; }
      void AddStmt(StmtNode* stmt) { stmts_.PushBack(stmt); }

      VariableDecl* GetVariable(int index);
      StmtNode* GetStmt(int index);

      int GetVarSize() { return vars_.GetSize(); }
      int GetStmtSize() { return stmts_.GetSize(); }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class CaseNode : public StmtNode {
    protected:
      ExprNodes values_;
      StmtNode* body_;

    public:
      CaseNode() {
        kind_ = CaseNodeTy;
        body_ = nullptr;
      }

      CaseNode(ExprNodes* values, StmtNode* body) {
        kind_ = CaseNodeTy;
        if (values) 
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

      void AddCase(ExprNode* value) { if(value) values_.PushBack(value);}
      void SetBody(StmtNode* body) { body_ = body; }

      int GetCaseValueNum() { return values_.GetSize(); }
      ExprNode* GetCaseValue(int index) { return values_[index]; }
      StmtNode* GetBody() { return body_; }
      bool IsDefaultCase() { return values_.IsEmpty(); }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class SwitchNode : public StmtNode {
    protected:
      ExprNode* cond_; // case condition
      CaseNodes case_values_;

    public:
      SwitchNode() {
        kind_ = SwitchNodeTy;
        cond_ = nullptr;
      }

      SwitchNode(ExprNode* cond, CaseNodes* case_values) {
        kind_ = SwitchNodeTy;
        cond_ = cond;
        if(case_values)
          case_values_ = *case_values;
      }

      virtual ~SwitchNode() {
        if (cond_)
          delete cond_;
        if (!case_values_.IsEmpty()) {
          for (int i = 0; i < case_values_.GetSize(); i++) {
            delete case_values_[i];
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
      void SetCases(CaseNodes* case_values) { if(case_values) case_values_ = *case_values; }

      ExprNode* GetCond() { return cond_; }
      int GetCaseNum() { return case_values_.GetSize(); }
      CaseNode* GetCase(int index) { return case_values_[index]; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class BreakNode : public StmtNode {
    protected:
    public:
      BreakNode() {
        kind_ = BreakNodeTy;
      }
      virtual ~BreakNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == BreakNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ContinueNode : public StmtNode {
    protected:
    public:
      ContinueNode() {
        kind_ = ContinueNodeTy;
      }
      virtual ~ContinueNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ContinueNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class GotoNode : public StmtNode {
    protected:
      std::string target_;
    public:
      GotoNode() {
        kind_ = GotoNodeTy;
      }
      GotoNode(const char* target) {
        kind_ = GotoNodeTy;
        target_ = target;
      }
      virtual ~GotoNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == GotoNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetTarget(const char* target) { target_ = target; }
      const char* GetTarget() { return target_.c_str(); }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ReturnNode : public StmtNode {
    protected:
      ExprNode* expr_;
    public:
      ReturnNode() {
        kind_ = ReturnNodeTy;
        expr_ = nullptr;
      }
      ReturnNode(ExprNode* expr) {
        kind_ = ReturnNodeTy;
        expr_ = expr;
      }
      virtual ~ReturnNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ReturnNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetExpr(ExprNode* expr) { expr_ = expr; }
      ExprNode* GetExpr() { return expr_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class AbstractAssignNode : public ExprNode {
    protected:
      ExprNode* lhs_, *rhs_;

      AbstractAssignNode() {
        kind_ = AbstractAssignNodeTy;
      }
    public:
      virtual ~AbstractAssignNode() {}
      
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == AbstractAssignNodeTy|| kind == ExprNodeTy ||
            kind == BaseNodeTy)
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
            kind == ExprNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == ExprNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetOp(AssignOp op) { op_ = op; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetOp(BinOp op) { bin_op_ = op; }
      void SetLeft(ExprNode* left) { left_ = left; }
      void SetRight(ExprNode* right) { right_ = right; }

      BinOp     GetOp() { return bin_op_; }
      ExprNode* GetLeft() { return left_; }
      ExprNode* GetRight() { return right_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
        if (kind == CastNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond_expr) { cond_expr_ = cond_expr; }
      void SetThen(ExprNode* then_expr) { then_expr_ = then_expr; }
      void SetElse(ExprNode* else_expr) { else_expr_ = else_expr; }

      ExprNode* GetCond() { return cond_expr_; }
      ExprNode* GetThen() { return then_expr_; }
      ExprNode* GetElse() { return else_expr_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ArgsNode : public BaseNode {
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
        if (kind == ArgsNodeTy || kind == BaseNodeTy)
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }
      
      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class LHSNode : public ExprNode {
    protected:
      LHSNode() {
        kind_ = LHSNodeTy;
      }
    public:
      virtual ~LHSNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LHSNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ArrayRefNode : public LHSNode {
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class LiteralNode : public ExprNode {
    protected:
      LiteralNode() {
        kind_ = LiteralNodeTy;
      }
    public:
      virtual ~LiteralNode() {}
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
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
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class RealLiteralNode : public LiteralNode {
    protected:
    double value_;
    
    public:
      RealLiteralNode() { kind_ = RealLiteralNodeTy; }
      RealLiteralNode(long value) { 
        kind_ = RealLiteralNodeTy;
        value_ = value; 
      }
      virtual ~RealLiteralNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == RealLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetValue(double v) { value_ = v; }
      long GetValue() { return value_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class TypeDefinition : public BaseNode {
    protected:
      std::string type_name_;
      TypeNode* type_;

      TypeDefinition() {
        kind_ = TypeDefinitionTy;
        type_ = nullptr;
      }
    public:
      virtual ~TypeDefinition() {
        if (type_)
          delete type_;
      }

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == TypeDefinitionTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetTypeName(const char* type_name) { type_name_ = type_name; }
      void SetType(TypeNode* ty) { type_ = ty; }

      const char* GetTypeName() { return type_name_.c_str(); }
      TypeNode* GetType() { return type_; }
  };

  class CompositeTypeDefinition : public TypeDefinition {
    protected:
      VariableDecls member_variables_;
      FunctionDecls member_functions_;

      CompositeTypeDefinition() {
        kind_ = CompositeTypeDefinitionTy;
      }
    public:
      virtual ~CompositeTypeDefinition() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == CompositeTypeDefinitionTy|| kind == TypeDefinitionTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class TypedefNode : public TypeDefinition {
    protected:
    public:
      TypedefNode() {
        kind_ = TypedefNodeTy;
        type_ = nullptr;
      }

      TypedefNode(TypeNode* ori_ty, const char* new_type_name) {
        kind_ = TypedefNodeTy;
        type_ = ori_ty;
        type_name_ = new_type_name;
      }

      virtual ~TypedefNode() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == TypedefNodeTy || kind == TypeDefinitionTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class FunctionDecl : public BaseNode {
    protected:
      bool is_static_; // storage
      TypeNode* ret_ty_; // return type
      std::string name_; // function name
      ParamNodes params_; // parameters
      BlockNode* body_; // body

    public:
      FunctionDecl();
      FunctionDecl(bool storage, TypeNode* retty, const char* fnname, 
          ParamNodes* params, BlockNode* body); 
      virtual ~FunctionDecl();

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == FunctionDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) { is_static_ = st; }
      void SetReturnType(TypeNode* ty) { ret_ty_ = ty; }
      void SetName(const char* fnname) { name_ = fnname; }
      void SetParams(ParamNodes* params);
      void SetBody(BlockNode* bd) { body_ = bd; }

      bool IsStatic() { return is_static_; }
      TypeNode* GetReturnType() { return ret_ty_; }
      const char* GetName() { return name_.c_str(); }
      ParamNode* GetParamNode(unsigned int index);
      BlockNode* GetBody() { return body_; }

      //FunctionType* GetType(); // TODO: need to get funciton type
      
      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };
  
  class VariableDecl : public BaseNode {
    bool is_static_;
    TypeNode* type_;
    std::string name_;
    ExprNode* initializer_;

    public:
      VariableDecl() {
        kind_ = VariableDeclTy;
        is_static_ = false;
        type_ = nullptr;
        initializer_ = nullptr;
      }
      VariableDecl(TypeNode* type, const char* name, ExprNode* init);

      virtual ~VariableDecl() {
        if (type_)
          delete type_;
        if (initializer_)
          delete initializer_;
      }
      virtual bool IsKindOf(NodeKind kind) {
        if (kind == VariableDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) {is_static_ = st;}
      void SetType(TypeNode* type) { type_ = type; }
      void SetName(const char* name) { name_ = name; }
      void SetName(const char* name, int len);
      void SetInit(ExprNode* init) { initializer_ = init; }

      bool HasInitializer() { return initializer_ ? true : false; }
      bool IsStatic()  { return is_static_; }
      bool GetType() { return type_; }
      const char* GetName() { return name_.c_str(); }
      ExprNode* GetInitializer() { return initializer_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ConstantDecl : public BaseNode {
    TypeNode* type_;
    std::string name_;
    ExprNode* initializer_;

    public:
      ConstantDecl() {
        kind_ = ConstantDeclTy;
        type_ = nullptr;
        initializer_ = nullptr;
      }
      ConstantDecl(TypeNode* type, const char* name, ExprNode* init);
      virtual ~ConstantDecl() {}

      virtual bool IsKindOf(NodeKind kind) {
        if (kind == ConstantDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetType(TypeNode* type) { type_ = type; }
      void SetName(const char* name) { name_ = name; }
      void SetName(const char* name, int len);
      void SetInit(ExprNode* init) { initializer_ = init; }

      TypeNode* GetType() { return type_; }
      const char* GetName() { return name_.c_str(); }
      ExprNode* GetInit() { return initializer_; }

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ClassNode : public CompositeTypeDefinition {
    public:
      ClassNode() {
        kind_ = ClassNodeTy;
      }
      ClassNode(const char* type_name, TypeNode* ty, VariableDecls* mem_var, 
          FunctionDecls* mem_func);

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

      virtual bool Accept(VisitorBase* visitor) override {
        return visitor->Visit(this);
      }
  };

  class Declarations {
    private:
      FunctionDecls funcs_;
      ConstantDecls conss_;
      VariableDecls vars_;
      ClassNodes    classes_;
      TypedefNodes  typedefs_;
      ImportNodes   imports_;

    public:
      Declarations();
      ~Declarations();

      void AddFunction(FunctionDecl* node) { funcs_.PushBack(node); }
      void AddConstant(ConstantDecl* node) { conss_.PushBack(node); }
      void AddVariable(VariableDecl* node) { vars_.PushBack(node); }
      void AddClass(ClassNode* node) { classes_.PushBack(node); }
      void AddTypedef(TypedefNode* node) { typedefs_.PushBack(node); }
      void AddImport(ImportNode* node) { imports_.PushBack(node); }

      int GetFunctionSize() { return funcs_.GetSize(); }
      int GetConstantSize() { return conss_.GetSize(); }
      int GetVariableSize() { return vars_.GetSize(); }
      int GetClassSize() { return classes_.GetSize(); }
      int GetTypedefSize() { return typedefs_.GetSize(); }
      int GetImportSize() { return imports_.GetSize(); }

      FunctionDecl* GetFunction(int index) { return funcs_[index];}
      ConstantDecl* GetConstant(int index) { return conss_[index];}
      VariableDecl* GetVariable(int index) { return vars_[index]; }
      ClassNode* GetClass(int index) { return classes_[index]; }
      TypedefNode* GetTypedef(int index) { return typedefs_[index]; }
      ImportNode* GetImport(int index) { return imports_[index]; }
  };


};
#endif
