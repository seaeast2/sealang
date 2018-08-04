#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_type.h"
#include "ast_node.h"
#include "core/hash.h"
#include "environment.h"


namespace AST {
  // AST context manager
  class ASTContext {
    protected:
      HashTable<Type*, 64> type_table_;
      Declarations* decls_;

      Scope top_scope_;
      Scope* cur_scope_;

      Environment global_env_;

    public:
      ASTContext();
      ~ASTContext();
      
      void SetLocalDecl(Declarations* decls);
      Declarations* GetLocalDecl();

      // Type environment control
      bool AddType(Type* ty);
      bool RemoveType(const char* type_name);
      Type* GetType(const char* type_name);

      // Scope control
      Scope* AddSiblingScope();
      Scope* AddChildScope();
      Scope* GetCurScope();
      void SetCurScope(Scope* scp);
      void AddNamedDecl(NamedDecl* decl);
      NamedDecl* GetDecl(const char* variable_name);

      // Variable resolver
      void ResolveVar();
      void PrintAST();
  };

};

#endif
