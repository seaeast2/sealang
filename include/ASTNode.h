#ifndef _ast_node_h_
#define _ast_node_h_

#include <cstring>
#include <assert.h>
#include "ASTType.h"
#include "Common.h"
#include "ASTVisitor.h"


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
          NamedDeclTy,
            FunctionDeclTy,
            VariableDeclTy,
              ParamDeclTy,
            ConstantDeclTy,
          TypeNodeTy,
          ImportNodeTy,
          ArgsNodeTy, // function call arguments

          // Statements
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
            BreakNodeTy,
            ContinueNodeTy,
            GotoNodeTy,
            ReturnNodeTy,

          // Expressions
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
            LValueNodeTy,
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

          // User defined types
          TypeDefinitionTy,
            CompositeTypeDefinitionTy,
              RecordDeclTy,
            TypedefNodeTy,
      };
    protected:
      NodeKind kind_;
      Location loc_;

      BaseNode() { kind_ = BaseNodeTy; }
    public:
      virtual ~BaseNode() {}

      NodeKind GetNodeKind() { return kind_; }
      virtual bool IsKindOf(NodeKind kind) const = 0;
      virtual bool Accept(ASTVisitor* visitor);
  };

  class TypeNode : public BaseNode {
    AST::Type* type_;
    public:
      TypeNode() {
        kind_ = TypeNodeTy;
        type_ = nullptr;
      }
      TypeNode(AST::Type* ty) {
        kind_ = TypeNodeTy;
        type_ = ty;
      }

      virtual ~TypeNode() {}
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == TypeNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetType(Type* ty) { type_ = ty; }
      AST::Type* GetType() { return type_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual ~ImportNode() {
        delete[] complete_path_;
      }
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ImportNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void AddImportPath(const char* path) { import_paths_.PushBack(path); }
      const char* GetImportPath();
      void Reverse() { import_paths_.Reverse(); }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ExprNodeTy|| 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      bool IsAssignable() const {
        return IsKindOf(LValueNodeTy);
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
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == BlockNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetVariables(VariableDecls* vars) { if (vars) vars_ = *vars; }
      void AddVariable(VariableDecl* var) { vars_.PushBack(var); }
      void SetStmts(StmtNodes* stmts) { if (stmts) stmts_ = *stmts; }
      void AddStmt(StmtNode* stmt) { stmts_.PushBack(stmt); }

      VariableDecl* GetVariableDecl(int index);
      StmtNode* GetStmt(int index);

      int GetVarNum() { return vars_.GetSize(); }
      int GetStmtNum() { return stmts_.GetSize(); }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ExprStmtNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetExpr(ExprNode* expr) { expr_ = expr; }
      ExprNode* GetExpr() { return expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == WhileNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetBody(StmtNode* body) { body_ = body; }

      ExprNode* GetCond() { return cond_; }
      StmtNode* GetBody() { return body_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == DoWhileNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetCond(ExprNode* cond) { cond_ = cond; }
      void SetBody(StmtNode* body) { body_ = body; }

      ExprNode* GetCond() { return cond_; }
      StmtNode* GetBody() { return body_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == BreakNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ContinueNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == GotoNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetTarget(const char* target) { target_ = target; }
      const char* GetTarget() { return target_.c_str(); }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual ~ReturnNode() {
        if (expr_)
          delete expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ReturnNodeTy || kind == StmtNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetExpr(ExprNode* expr) { expr_ = expr; }
      ExprNode* GetExpr() { return expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class AbstractAssignNode : public ExprNode {
    protected:
      ExprNode* lhs_, *rhs_;

      AbstractAssignNode() {
        kind_ = AbstractAssignNodeTy;
        lhs_ = nullptr;
        rhs_ = nullptr;
      }
    public:
      virtual ~AbstractAssignNode() {
        if(lhs_)
          delete lhs_;
        if(rhs_)
          delete rhs_;
      }
      
      virtual bool IsKindOf(NodeKind kind) const {
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
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == AssignNodeTy || kind == AbstractAssignNodeTy|| 
            kind == ExprNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == OpAssignNodeTy || kind == AbstractAssignNodeTy|| 
            kind == ExprNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetOp(AssignOp op) { op_ = op; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class AddressNode : public ExprNode {
    ExprNode * expr_;
    public:
      AddressNode() {
        kind_ = AddressNodeTy;
        expr_ = nullptr;
      }
      AddressNode(ExprNode* expr) {
        kind_ = AddressNodeTy;
        expr_ = expr;
      }
      virtual ~AddressNode() {
        if (expr_)
          delete expr_;
      }
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == AddressNodeTy || 
            kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetExpr(ExprNode* expr) { expr_ = expr; }
      ExprNode* GetExpr() { return expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
        left_ = nullptr;
        right_ = nullptr;
      }
      BinaryOpNode(ExprNode* left, BinOp op, ExprNode* right) {
        kind_ = BinaryOpNodeTy;
        bin_op_ = op;
        left_ = left;
        right_ = right;
      }
      virtual ~BinaryOpNode() {
        if (left_)
          delete left_;
        if (right_)
          delete right_;
      }
      
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
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
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == LogicalAndNodeTy ||
            kind == BinaryOpNodeTy || kind == ExprNodeTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == LogicalOrNodeTy ||
            kind == BinaryOpNodeTy || kind == ExprNodeTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual ~CastNode() {
        if (cast_type_)
          delete cast_type_;
        if (term_expr_)
          delete term_expr_;
      }
      
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == CastNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      TypeNode* GetCastType() { return cast_type_; }
      ExprNode* GetTermExpr() { return term_expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual ~CondExprNode() {
        if (cond_expr_)
          delete cond_expr_;
        if (then_expr_)
          delete then_expr_;
        if (else_expr_)
          delete else_expr_;
      }
      
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ArgsNode : public BaseNode {
    SimpleVector<ExprNode*> args_;
    public:
      ArgsNode() {
        kind_ = ArgsNodeTy;
      }
      virtual ~ArgsNode() {
        for (int i = 0; i < args_.GetSize(); i++) {
          delete args_[i];
        }
      }
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ArgsNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      int Add(ExprNode* arg) {
        args_.PushBack(arg);
        return args_.GetSize();
      }
      int GetArgNum() {
        return args_.GetSize();
      }
      ExprNode* GetArg(int idx) {
        if (idx < 0 || idx > args_.GetSize()-1)
          return nullptr;
        return args_[idx];
      }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class FuncCallNode : public ExprNode {
    ExprNode* func_expr_;
    ArgsNode* args_;
    public:
      FuncCallNode() {
        kind_ = FuncCallNodeTy;
        func_expr_ = nullptr;
        args_ = nullptr;
      }

      FuncCallNode(ExprNode* func, ArgsNode* args) {
        kind_ = FuncCallNodeTy;
        func_expr_ = func;
        args_ = args;
      }

      virtual ~FuncCallNode() {
        if(func_expr_)
          delete func_expr_;
        if (args_)
          delete args_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == FuncCallNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      const ExprNode* GetFuncExpr() const { return func_expr_; }
      ArgsNode* GetArgs() { return args_; }
      
      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class LValueNode : public ExprNode {
    protected:
      LValueNode() {
        kind_ = LValueNodeTy;
      }
    public:
      virtual ~LValueNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == LValueNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }
  };

  class ArrayRefNode : public LValueNode {
    ExprNode* array_base_expr_; 
    ExprNode* array_size_expr_;
    int array_size_;
    public:
      ArrayRefNode() {
        kind_ = ArrayRefNodeTy;
        array_size_ = 0;
        array_base_expr_ = nullptr;
        array_size_expr_ = nullptr;
      }
      ArrayRefNode(ExprNode* arr_base_expr, ExprNode* arr_size) {
        kind_ = ArrayRefNodeTy;
        array_size_ = 0;
        array_size_expr_ = arr_size;
        array_base_expr_ = arr_base_expr;
      }
      virtual ~ArrayRefNode() {
        if (array_base_expr_)
          delete array_base_expr_;
        if (array_size_expr_)
          delete array_size_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ArrayRefNodeTy ||
            kind == LValueNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetArrayBaseExpr() { return array_base_expr_; }
      ExprNode* GetArraySizeExpr() { return array_size_expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class DereferenceNode : public LValueNode {
    ExprNode* base_expr_;
    public:
      DereferenceNode() {
        kind_ = DereferenceNodeTy;
        base_expr_ = nullptr;
      }
      DereferenceNode(ExprNode* base_expr) {
        kind_ = DereferenceNodeTy;
        base_expr_ = base_expr;
      }
      virtual ~DereferenceNode() {
        if (base_expr_)
          delete base_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == DereferenceNodeTy || 
            kind == LValueNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetBaseExpr() { return base_expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class MemberRefNode : public LValueNode {
    ExprNode* base_expr_;
    std::string member_name_;
    public:
      MemberRefNode() {
        kind_ = MemberRefNodeTy;
        base_expr_ = nullptr;
      }
      MemberRefNode(ExprNode* base_expr, const char* mbname) { 
        kind_ = MemberRefNodeTy;
        base_expr_ = base_expr;
        member_name_ = mbname;
      }
      virtual ~MemberRefNode() {
        if (base_expr_)
          delete base_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == MemberRefNodeTy ||
            kind == LValueNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetBaseExpr() { return base_expr_; }
      const char* GetMemberName() { return member_name_.c_str(); }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class PtrMemberRefNode : public LValueNode {
    ExprNode* base_expr_;
    std::string member_name_;
    public:
      PtrMemberRefNode() {
        kind_ = PtrMemberRefNodeTy;
        base_expr_ = nullptr;
      }
      PtrMemberRefNode(ExprNode* base_expr, const char* mbname) { 
        kind_ = PtrMemberRefNodeTy;
        base_expr_ = base_expr;
        member_name_ = mbname;
      }
      virtual ~PtrMemberRefNode() {
        if (base_expr_)
          delete base_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == PtrMemberRefNodeTy ||
            kind == LValueNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetBaseExpr() { return base_expr_; }
      const char* GetMemberName() { return member_name_.c_str(); }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  // Function or Variable can be a VariableNode.
  class VariableNode : public LValueNode {
    NamedDecl* defined_decl_;
    std::string variable_name_;
    public:
      VariableNode() {
        defined_decl_ = nullptr;
        kind_ = VariableNodeTy;
      }
      VariableNode(const char* name) {
        defined_decl_ = nullptr;
        kind_ = VariableNodeTy;
        variable_name_ = name;
      }
      virtual ~VariableNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == VariableNodeTy || kind == LValueNodeTy || 
            kind == ExprNodeTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetNamedDecl(NamedDecl* nd) { defined_decl_ = nd; }

      const char* GetVarName() { return variable_name_.c_str(); }
      NamedDecl* GetNamedDecl() { return defined_decl_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class SizeofExprNode : public ExprNode {
    ExprNode* size_expr_;
    public:
      SizeofExprNode() {
        kind_ = SizeofExprNodeTy;
        size_expr_ = nullptr;
      }
      SizeofExprNode(ExprNode* expr) {
        kind_ = SizeofExprNodeTy;
        size_expr_ = expr;
      }
      virtual ~SizeofExprNode() {
        if (size_expr_)
          delete size_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == SizeofExprNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetSizeExpr() { return size_expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class SizeofTypeNode : public ExprNode {
    ExprNode * size_expr_;
    public:
      SizeofTypeNode() {
        kind_ = SizeofTypeNodeTy;
        size_expr_ = nullptr;
      }

      SizeofTypeNode(ExprNode* expr) {
        kind_ = SizeofTypeNodeTy;
        size_expr_ = expr;
      }
      
      virtual ~SizeofTypeNode() {
        if (size_expr_)
          delete size_expr_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == SizeofTypeNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      ExprNode* GetSizeExpr() { return size_expr_; }

      virtual bool Accept(ASTVisitor* visitor) override {
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
    ExprNode * base_expr_;
    UnaryOp op_;

    public:
      UnaryOpNode() {
        kind_ = UnaryOpNodeTy;
        base_expr_ = nullptr;
      }

      UnaryOpNode(ExprNode* expr, UnaryOpNode::UnaryOp op) {
        kind_ = UnaryOpNodeTy;
        base_expr_ = expr;
        op_ = op;
      }

      virtual ~UnaryOpNode() {
        if (base_expr_)
          delete base_expr_;
      }

      ExprNode* GetBaseExpr() { return base_expr_; }
      UnaryOp GetOp() { return op_; }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class UnaryArithmeticOpNode : public UnaryOpNode{
    public:
      UnaryArithmeticOpNode() {
        kind_ = UnaryArithmeticOpNodeTy;
      }
      virtual ~UnaryArithmeticOpNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class PrefixOpNode : public UnaryArithmeticOpNode{
    public:
      PrefixOpNode() {
        kind_ = PrefixOpNodeTy;
      }
      PrefixOpNode(ExprNode *base_expr, UnaryOp op) {
        kind_ = PrefixOpNodeTy;
        base_expr_ = base_expr; 
        op_ = op;
      }
      virtual ~PrefixOpNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == PrefixOpNodeTy ||
            kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class SuffixOpNode : public UnaryArithmeticOpNode{
    public:
      SuffixOpNode() {
        kind_ = SuffixOpNodeTy;
      }
      SuffixOpNode(ExprNode *base_expr, UnaryOp op) {
        kind_ = SuffixOpNodeTy;
        base_expr_ = base_expr; 
        op_ = op;
      }
      virtual ~SuffixOpNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == SuffixOpNodeTy ||
            kind == UnaryArithmeticOpNodeTy ||
            kind == UnaryOpNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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
      virtual bool IsKindOf(NodeKind kind) const {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == IntegerLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == RealLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetValue(double v) { value_ = v; }
      long GetValue() { return value_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class StringLiteralNode : public LiteralNode {
    private:
      std::string str_;
    
    public:
      StringLiteralNode() { kind_ = StringLiteralNodeTy; }
      StringLiteralNode(const char* str, int str_len) { 
        kind_ = StringLiteralNodeTy; 
        str_ = std::string(str, str_len);
      }
      virtual ~StringLiteralNode() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == StringLiteralNodeTy ||
            kind == LiteralNodeTy || kind == ExprNodeTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      const char* GetStr() { return str_.c_str(); }

      virtual bool Accept(ASTVisitor* visitor) override {
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == TypeDefinitionTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetTypeName(const char* type_name) { type_name_ = type_name; }
      void SetType(TypeNode* ty) { type_ = ty; }

      const char* GetTypeName() { return type_name_.c_str(); }
      TypeNode* GetTypeNode() { return type_; }
  };

  class CompositeTypeDefinition : public TypeDefinition {
    protected:
      VariableDecls memberVariableDecls_;
      FunctionDecls memberFunctionDecls_;

      CompositeTypeDefinition() {
        kind_ = CompositeTypeDefinitionTy;
      }
    public:
      virtual ~CompositeTypeDefinition();

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == CompositeTypeDefinitionTy|| kind == TypeDefinitionTy || kind == BaseNodeTy)
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

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == TypedefNodeTy || kind == TypeDefinitionTy ||
            kind == BaseNodeTy)
          return true;
        return false;
      }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class NamedDecl : public BaseNode {
    protected:
      std::string name_; // Value name
      TypeNode* typeNode_; // Type

      NamedDecl() {
        kind_ = NamedDeclTy;
        typeNode_ = nullptr;
      }
    public:
      virtual ~NamedDecl() {
        if (typeNode_)
          delete typeNode_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == NamedDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetName(const char* fnname) { name_ = fnname; }
      void SetName(const char* name, int len) {
        std::string str(name, len);
        name_ = str;
      }
      const char* GetName() const { return name_.c_str(); }

      // TypeNode interface
      void SetTypeNode(TypeNode* typeNode) { typeNode_ = typeNode; }
      TypeNode* GetTypeNode() { return typeNode_; }
  };

  class FunctionDecl : public NamedDecl {
    protected:
      // TODO: Need to implement member function
      // parent class in case function is class member.
      RecordDecl* thisClass_; 
      bool is_static_; // storage
      TypeNode* retType_; // return type
      ParamDecls params_; // parameters
      BlockNode* body_; // body

    public:
      FunctionDecl();
      FunctionDecl(bool storage, TypeNode* retty, const char* fnname, 
          ParamDecls* params, BlockNode* body, RecordDecl* thisClass = nullptr); 
      virtual ~FunctionDecl();

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == FunctionDeclTy || kind == NamedDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) { is_static_ = st; }
      void SetReturnTypeNode(TypeNode* ty) { retType_ = ty; }
      void SetParams(ParamDecls* params);
      void SetBody(BlockNode* bd) { body_ = bd; }
      void SetThisClass(RecordDecl* RD) { thisClass_= RD; }

      bool IsStatic() { return is_static_; }

      TypeNode* GetReturnTypeNode() { return retType_; }
      int GetParamNum() { return params_.GetSize(); }
      ParamDecl* GetParamNode(int index);
      BlockNode* GetBody() { return body_; }

      // for parent class
      RecordDecl* GetThisClass() { return thisClass_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };
  
  // Variable declaration 
  class VariableDecl : public NamedDecl {
    bool is_static_;
    ExprNode* initializer_;

    public:
      VariableDecl() {
        kind_ = VariableDeclTy;
        is_static_ = false;
        initializer_ = nullptr;
      }
      VariableDecl(TypeNode* type, const char* name, ExprNode* init);

      virtual ~VariableDecl() {
        if (initializer_)
          delete initializer_;
      }
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == VariableDeclTy || kind == NamedDeclTy || 
            kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetStorage(bool st) {is_static_ = st;}
      void SetInit(ExprNode* init) { initializer_ = init; }

      bool HasInitializer() { return initializer_ ? true : false; }
      bool IsStatic()  { return is_static_; }
      ExprNode* GetInitializer() { return initializer_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ParamDecl : public VariableDecl {
    protected:
      bool var_arg_; // variable args
    public:
      ParamDecl() {
        kind_ = ParamDeclTy;
        var_arg_ = false;
      }
      ParamDecl(TypeNode* ty, const char* name, bool var_arg) {
        kind_ = ParamDeclTy;
        var_arg_ = var_arg;
        typeNode_ = ty;
        name_ = name;
      }
      virtual ~ParamDecl() {
      }
      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ParamDeclTy || kind == VariableDeclTy || 
            kind == NamedDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetVarArgs(bool var_arg) { var_arg_ = var_arg; }

      bool GetVarArgs() { return var_arg_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class ConstantDecl : public NamedDecl {
    ExprNode* initializer_;

    public:
      ConstantDecl() {
        kind_ = ConstantDeclTy;
        initializer_ = nullptr;
      }
      ConstantDecl(TypeNode* type, const char* name, ExprNode* init);
      virtual ~ConstantDecl() {
        if (initializer_)
          delete initializer_;
      }

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == ConstantDeclTy || kind == NamedDeclTy || kind == BaseNodeTy)
          return true;
        return false;
      }

      void SetInit(ExprNode* init) { initializer_ = init; }

      ExprNode* GetInitializer() { return initializer_; }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };


  class RecordDecl : public CompositeTypeDefinition {
    public:
      RecordDecl() {
        kind_ = RecordDeclTy;
      }
      RecordDecl(const char* type_name, TypeNode* ty, VariableDecls* mem_var, FunctionDecls* mem_func);

      virtual ~RecordDecl() {}

      virtual bool IsKindOf(NodeKind kind) const {
        if (kind == RecordDeclTy || kind == CompositeTypeDefinitionTy ||
            kind == TypeDefinitionTy|| kind == BaseNodeTy)
          return true;
        return false;
      }

      void AddMemVariableDecl(VariableDecl* var) { memberVariableDecls_.PushBack(var); }
      void AddMemFunctionDecl(FunctionDecl* fun) { 
        fun->SetThisClass(this);
        memberFunctionDecls_.PushBack(fun); 
      }

      VariableDecl* GetMemVariableDecl(int index);
      FunctionDecl* GetMemFunctionDecl(int index);

      int GetMemVarDeclNum() { return memberVariableDecls_.GetSize(); }
      int GetMemFunDeclNum() { return memberFunctionDecls_.GetSize(); }

      void ReverseVariableOrder() { memberVariableDecls_.Reverse(); }
      void ReverseFunctionOrder() { memberFunctionDecls_.Reverse(); }

      virtual bool Accept(ASTVisitor* visitor) override {
        return visitor->Visit(this);
      }
  };

  class Declarations {
    private:
      // TODO : Need to add external defined function and variables from import.
      ImportNodes   importNodes_;
      TypedefNodes  typedefNodes_;

      ConstantDecls constDecls_;
      VariableDecls varDecls_;
      RecordDecls   recordDecls_;
      FunctionDecls funcDecls_;

    public:
      Declarations();
      ~Declarations();

      void AddFunctionDecl(FunctionDecl* node) { funcDecls_.PushBack(node); }
      void AddConstantDecl(ConstantDecl* node) { constDecls_.PushBack(node); }
      void AddVariableDecl(VariableDecl* node) { varDecls_.PushBack(node); }
      //void AddClass(ClassNode* node) { classes_.PushBack(node); }
      void AddRecordDecl(RecordDecl* RD) { recordDecls_.PushBack(RD); }
      void AddTypedefNode(TypedefNode* node) { typedefNodes_.PushBack(node); }
      void AddImportNode(ImportNode* node) { importNodes_.PushBack(node); }

      int GetFunctionDeclNum() { return funcDecls_.GetSize(); }
      int GetConstantDeclNum() { return constDecls_.GetSize(); }
      int GetVariableDeclNum() { return varDecls_.GetSize(); }
      int GetRecordDeclNum() { return recordDecls_.GetSize(); }
      int GetTypedefNodeNum() { return typedefNodes_.GetSize(); }
      int GetImportNodeNum() { return importNodes_.GetSize(); }

      FunctionDecl* GetFunctionDecl(int index) { return funcDecls_[index];}
      ConstantDecl* GetConstantDecl(int index) { return constDecls_[index];}
      VariableDecl* GetVariableDecl(int index) { return varDecls_[index]; }
      RecordDecl* GetRecordDecl(int index) { return recordDecls_[index]; }
      TypedefNode* GetTypedefNode(int index) { return typedefNodes_[index]; }
      ImportNode* GetImportNode(int index) { return importNodes_[index]; }
  };


};
#endif
