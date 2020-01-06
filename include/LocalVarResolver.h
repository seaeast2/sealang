#ifndef _LocalVarResolver_h_
#define _LocalVarResolver_h_

#include "ASTNode.h"
#include "ASTVisitor.h"
#include "Scope.h"

namespace AST {

  // 1. Check duplication of local variable.
  // 2. Check linkage between Variable defintion and Variable instance.
  class LocalVarResolver : public ASTVisitor {
    Declarations* decls_;
    Scope* scope_;
    // TODO : Need to add ErrorHandler

    bool is_function_body_;
    
    public:
      LocalVarResolver();
      virtual ~LocalVarResolver();

      void PushScope();
      void PopScope();
      bool Resolve(Declarations* decl, Scope* scp);

      // Statements
      virtual bool Visit(BlockNode* node) override;
      virtual bool Visit(VariableNode* node) override;

      // Etc
      virtual bool Visit(FunctionDecl* node) override;
      virtual bool Visit(ClassNode* node) override;   
  };
};

#endif
