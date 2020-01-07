#include <string>
#include "ASTNode.h"


using namespace std;

namespace AST {
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

  FunctionDecl::FunctionDecl(bool storage, TypeNode* retty, const char* fnname, 
      ParamNodes* params, BlockNode* body, RecordDecl* thisClass) {
    kind_ = FunctionDeclTy;
    is_static_ = storage;
    retType_ = retty;
    name_ = fnname;

    for (int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }

    body_ = body;
    thisClass_ = thisClass;
  }

  FunctionDecl::~FunctionDecl() {
    ParamNode* param_tmp = nullptr;

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

  void FunctionDecl::SetParams(ParamNodes* params) {
    for(int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }
  }
  
  ParamNode* FunctionDecl::GetParamNode(int index) {
    if (index >= params_.GetSize())
      return nullptr;
    return params_[index];
  }


  // VariableDecl ====================================
  VariableDecl::VariableDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = VariableDeclTy;
    type_ = type;
    name_ = name;
    initializer_ = init;
  }

  // ConstantDecl =======================================
  ConstantDecl::ConstantDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = ConstantDeclTy;
    type_ = type;
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
  /*ClassNode::ClassNode(const char* type_name, TypeNode* ty, VariableDecls* mem_var, 
      FunctionDecls* mem_func) {
    kind_ = ClassNodeTy;

    type_name_ = type_name;
    type_ = ty;

    member_variables_ = *mem_var;
    member_functions_ = *mem_func;
  }


  VariableDecl* ClassNode::GetMemVariable(int index) { 
    if (index >= member_variables_.GetSize())
      return nullptr;
    return member_variables_[index]; 
  }

  FunctionDecl* ClassNode::GetMemFunction(int index) { 
    if (index >= member_functions_.GetSize())
      return nullptr;
    return member_functions_[index]; 
  }*/
  
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
//    if (!classes_.IsEmpty()) {
//      for (int i = 0; i < classes_.GetSize(); i++) {
//        ClassNode* cn = classes_[i];
//        delete cn;
//      }
//    }
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

