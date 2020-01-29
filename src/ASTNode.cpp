#include <string>
#include "ASTNode.h"


using namespace std;

namespace AST {
  // BaseNode ==================================
  bool BaseNode::Accept(ASTVisitor* visitor) {
    switch (kind_) {
      case BlockNodeTy:
        return visitor->Visit(reinterpret_cast<BlockNode*>(this));
      case LabelNodeTy:
        return visitor->Visit(reinterpret_cast<LabelNode*>(this));
      case ExprStmtNodeTy:
        return visitor->Visit(reinterpret_cast<ExprStmtNode*>(this));
      case IfNodeTy:
        return visitor->Visit(reinterpret_cast<IfNode*>(this));
      case WhileNodeTy:
        return visitor->Visit(reinterpret_cast<WhileNode*>(this));
      case DoWhileNodeTy:
        return visitor->Visit(reinterpret_cast<DoWhileNode*>(this));
      case ForNodeTy:
        return visitor->Visit(reinterpret_cast<ForNode*>(this));
      case CaseNodeTy:
        return visitor->Visit(reinterpret_cast<CaseNode*>(this));
      case SwitchNodeTy:
        return visitor->Visit(reinterpret_cast<SwitchNode*>(this));
      case BreakNodeTy:
        return visitor->Visit(reinterpret_cast<BreakNode*>(this));
      case ContinueNodeTy:
        return visitor->Visit(reinterpret_cast<ContinueNode*>(this));
      case GotoNodeTy:
        return visitor->Visit(reinterpret_cast<GotoNode*>(this));
      case ReturnNodeTy:
        return visitor->Visit(reinterpret_cast<ReturnNode*>(this));

      case AssignNodeTy:
        return visitor->Visit(reinterpret_cast<AssignNode*>(this));
      case OpAssignNodeTy:
        return visitor->Visit(reinterpret_cast<OpAssignNode*>(this));
      case AddressNodeTy:
        return visitor->Visit(reinterpret_cast<AddressNode*>(this));
      case BinaryOpNodeTy:
        return visitor->Visit(reinterpret_cast<BinaryOpNode*>(this));
      case LogicalAndNodeTy:
        return visitor->Visit(reinterpret_cast<LogicalAndNode*>(this));
      case LogicalOrNodeTy:
        return visitor->Visit(reinterpret_cast<LogicalOrNode*>(this));
      case CastNodeTy:
        return visitor->Visit(reinterpret_cast<CastNode*>(this));
      case CondExprNodeTy:
        return visitor->Visit(reinterpret_cast<CondExprNode*>(this));
      case FuncCallNodeTy:
        return visitor->Visit(reinterpret_cast<FuncCallNode*>(this));
      case ArrayRefNodeTy:
        return visitor->Visit(reinterpret_cast<ArrayRefNode*>(this));
      case PtrDereferenceNodeTy:
        return visitor->Visit(reinterpret_cast<PtrDereferenceNode*>(this));
      case MemberRefNodeTy:
        return visitor->Visit(reinterpret_cast<MemberRefNode*>(this));
      case PtrMemberRefNodeTy:
        return visitor->Visit(reinterpret_cast<PtrMemberRefNode*>(this));
      case VariableNodeTy:
        return visitor->Visit(reinterpret_cast<VariableNode*>(this));
      case IntegerLiteralNodeTy:
        return visitor->Visit(reinterpret_cast<IntegerLiteralNode*>(this));
      case RealLiteralNodeTy:
        return visitor->Visit(reinterpret_cast<RealLiteralNode*>(this));
      case StringLiteralNodeTy:
        return visitor->Visit(reinterpret_cast<StringLiteralNode*>(this));
      case SizeofExprNodeTy:
        return visitor->Visit(reinterpret_cast<SizeofExprNode*>(this));
      case SizeofTypeNodeTy:
        return visitor->Visit(reinterpret_cast<SizeofTypeNode*>(this));
      case UnaryOpNodeTy:
        return visitor->Visit(reinterpret_cast<UnaryOpNode*>(this));
      case PrefixOpNodeTy:
        return visitor->Visit(reinterpret_cast<PrefixOpNode*>(this));
      case SuffixOpNodeTy:
        return visitor->Visit(reinterpret_cast<SuffixOpNode*>(this));

      case FunctionDeclTy:
        return visitor->Visit(reinterpret_cast<FunctionDecl*>(this));
      case VariableDeclTy:
        return visitor->Visit(reinterpret_cast<VariableDecl*>(this));
      case ConstantDeclTy:
        return visitor->Visit(reinterpret_cast<ConstantDecl*>(this));
      case TypeNodeTy:
        return visitor->Visit(reinterpret_cast<TypeNode*>(this));
      case ParamDeclTy:
        return visitor->Visit(reinterpret_cast<ParamDecl*>(this));
      case ImportNodeTy:
        return visitor->Visit(reinterpret_cast<ImportNode*>(this));
      case ArgsNodeTy:
        return visitor->Visit(reinterpret_cast<ArgsNode*>(this));
      case RecordDeclTy:
        return visitor->Visit(reinterpret_cast<RecordDecl*>(this));
      case TypedefNodeTy:
        return visitor->Visit(reinterpret_cast<TypedefNode*>(this));

      default:
        assert(0&& "Error on Variable checker : Duplicate name");
        break;
    }
    return false;
  }
  
  // ImportNode ==================================
  const char* ImportNode::GetImportPath() {
    string tmp = ".";
    for (int i = 0; i < import_paths_.GetSize(); i++) {
      tmp += "/" + import_paths_[i];
    }
    
    if(complete_path_)
      delete[] complete_path_;

    complete_path_ = new char[tmp.size()+1];
    strcpy(complete_path_, tmp.c_str());
    return complete_path_;
  }

  // BlockNode ====================================
  BlockNode::~BlockNode() {
    VariableDecl* var_tmp = nullptr;
    StmtNode* stmt_tmp = nullptr;

    if (!vars_.IsEmpty()) {
      for (int i = 0; i < vars_.GetSize(); i++) {
        var_tmp = vars_[i];
        delete var_tmp;
      }
    }

    if (!stmts_.IsEmpty()) {
      for (int i = 0; i < stmts_.GetSize(); i++) {
        stmt_tmp = stmts_[i];
        delete stmt_tmp;
      }
    }
  }

  VariableDecl* BlockNode::GetVariableDecl(int index) { 
    if (index >= vars_.GetSize())
      return nullptr;
    return vars_[index]; 
  }

  StmtNode* BlockNode::GetStmt(int index) { 
    if (index >= stmts_.GetSize())
      return nullptr;
    return stmts_[index]; 
  }

  // FunctionDecl ====================================
  FunctionDecl::FunctionDecl() {
    kind_ = FunctionDeclTy;
    is_static_ = false;
    retType_ = nullptr;
    body_ = nullptr;
  }

  FunctionDecl::FunctionDecl(bool storage, TypeNode* retty, const char* fnname, ParamDecls* params, BlockNode* body, RecordDecl* thisClass) {
    kind_ = FunctionDeclTy;
    is_static_ = storage;
    retType_ = retty;
    name_ = fnname;

    // copy params
    for (int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }

    body_ = body;
    thisClass_ = thisClass;
  }

  FunctionDecl::~FunctionDecl() {
    ParamDecl* param_tmp = nullptr;

    if (retType_) 
      delete retType_;

    if (!params_.IsEmpty()) {
      for (int i = 0; i < params_.GetSize(); i++) {
        param_tmp = params_[i];
        delete param_tmp;
      }
    }

    if (body_)
      delete body_;
  }

  void FunctionDecl::SetParams(ParamDecls* params) {
    for(int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }
  }
  
  ParamDecl* FunctionDecl::GetParamNode(int index) {
    if (index >= params_.GetSize())
      return nullptr;
    return params_[index];
  }


  // VariableDecl ====================================
  VariableDecl::VariableDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = VariableDeclTy;
    typeNode_ = type;
    name_ = name;
    initializer_ = init;
  }

  // ConstantDecl =======================================
  ConstantDecl::ConstantDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = ConstantDeclTy;
    typeNode_ = type;
    name_ = name;
    initializer_ = init;
  }

  // CompositeTypeDefinition =======================================
  CompositeTypeDefinition::~CompositeTypeDefinition() {
    VariableDecl* var_tmp = nullptr;
    FunctionDecl* fun_tmp = nullptr;

    if (!memberVariableDecls_.IsEmpty()) {
      for (int i = 0; i < memberVariableDecls_.GetSize(); i++) {
        var_tmp = memberVariableDecls_[i];
        delete var_tmp;
      }
    }

    if (!memberFunctionDecls_.IsEmpty()) {
      for (int i = 0; i < memberFunctionDecls_.GetSize(); i++) {
        fun_tmp = memberFunctionDecls_[i];
        delete fun_tmp;
      }
    }
  }
  
  // RecordDecl =============================================
  RecordDecl::RecordDecl(const char* type_name, TypeNode* ty, VariableDecls* mem_var, 
      FunctionDecls* mem_func) {
    kind_ = RecordDeclTy;

    type_name_ = type_name;
    type_ = ty;

    memberVariableDecls_ = *mem_var;
    memberFunctionDecls_ = *mem_func;
  }


  VariableDecl* RecordDecl::GetMemVariableDecl(int index) { 
    if (index >= memberVariableDecls_.GetSize())
      return nullptr;
    return memberVariableDecls_[index]; 
  }

  FunctionDecl* RecordDecl::GetMemFunctionDecl(int index) { 
    if (index >= memberFunctionDecls_.GetSize())
      return nullptr;
    return memberFunctionDecls_[index]; 
  }
  // Declarations =============================================
  Declarations::Declarations() {
  }

  Declarations::~Declarations() {
    if (!funcDecls_.IsEmpty()) {
      for (int i = 0; i < funcDecls_.GetSize(); i++) {
        FunctionDecl* fd = funcDecls_[i];
        delete fd;
      }
    }
    if (!constDecls_.IsEmpty()) {
      for (int i = 0; i < constDecls_.GetSize(); i++) {
        ConstantDecl* cd = constDecls_[i];
        delete cd;
      }
    }
    if (!varDecls_.IsEmpty()) {
      for (int i = 0; i < varDecls_.GetSize(); i++) {
        VariableDecl* vd = varDecls_[i];
        delete vd;
      }
    }
    
    if (!recordDecls_.IsEmpty()) {
      for (int i = 0; i < recordDecls_.GetSize(); i++) {
        RecordDecl* RD = recordDecls_[i];
        delete RD;
      }
    }

    if (!typedefNodes_.IsEmpty()) {
      for (int i = 0; i < typedefNodes_.GetSize(); i++) {
        TypedefNode* tn = typedefNodes_[i];
        delete tn;
      }
    }

    if (!importNodes_.IsEmpty()) {
      for (int i = 0; i < importNodes_.GetSize(); i++) {
        ImportNode* IN = importNodes_[i];
        delete IN;
      }
    }
  }
};

