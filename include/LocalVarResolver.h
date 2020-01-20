#ifndef _LocalVarResolver_h_
#define _LocalVarResolver_h_

#include "ASTNode.h"
#include "ASTVisitor.h"
#include "Scope.h"

namespace AST {

  // 1. Check duplication of local variable in same scope.
  //    ex) {
  //           int aa;
  //           short aa;
  //        }
  // 2. Set VariableNode's declaration.
  //  ex)
  //  {
  //     int aa = 10; 
  //     {
  //        funcAA(aa); // set identifier aa's declaration to 'int a = 10'.
  //     }
  //  }
  class LocalVarResolver : public ASTVisitor {
    private:
      Declarations* decls_;
      Scope* currentScope_;
      // TODO : Need to add ErrorHandler

    public:
      LocalVarResolver();
      virtual ~LocalVarResolver();

      void PushScope();
      void PopScope();
      bool Resolve(Declarations* decl, Scope* scp);

      bool ResolveVariableInitializer();
      bool ResolveConstantInitializer();
      bool ResolveFunction();

      // Statements
      virtual bool Visit(BlockNode* node) override;
      virtual bool Visit(VariableNode* node) override;
      // Etc
      virtual bool Visit(FunctionDecl* node) override;
      //virtual bool Visit(ClassNode* node) override;   
  };
};

#endif
