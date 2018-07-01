#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_type.h"
#include "ast_node.h"
#include "environment.h"


namespace AST {
  // AST context manager
  class ASTContext {
    protected:
      Declarations* imported_decls_;
      Declarations* decls_;

      // type environment
      Environment<Type*> type_env_;
      // variable evironment
      Environment<VariableDecl*> var_evn_;

    public:
      ASTContext();
      ~ASTContext();
      
      void SetLocalDecl(Declarations* decl) { decls_ = decl; }
      void SetImportedDecl(Declarations* decl) { imported_decls_ = decl; }

      Declarations* GetLocalDecl() { return decls_; }
      Declarations* GetImportedDecl() { return imported_decls_; }

      // Type environment control
      Type* FindType(const char* type_name);
      bool AddType(const char* key, Type* ty);
      Type* RemoveType(const char* key);
  };

};

#endif
