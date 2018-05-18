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
      VecStr imports_;
      RootNode* rootNode_;

      // type environment
      Environment<Type*> type_env_;
      // variable evironment
      Environment<VariableNode*> var_evn_;

      // primitive types
      VoidType void_ty_;
      CharType char_ty_;
      ShortType short_ty_;
      IntType int_ty_;
      LongType long_ty_;
      FloatType float_ty_;
      DoubleType double_ty_;

    public:
      ASTContext();
      ~ASTContext();

      void AddImport(const std::string &import);
      VecStr::iterator import_begin();
      VecStr::iterator import_end();
      
      RootNode* GetAstRoot() { return rootNode_; }

      const VoidType* GetVoidTy() const {
        return &void_ty_;
      }
      const CharType* GetCharTy() const {
        return &char_ty_;
      }
      const ShortType* GetShortTy() const {
        return &short_ty_;
      }
      const IntType* GetIntTy() const {
        return &int_ty_;
      }
      const LongType* GetLongTy() const {
        return &long_ty_;
      }
      const FloatType* GetFloatTy() const {
        return &float_ty_;
      }
      const DoubleType* GetDoubleTy() const {
        return &double_ty_;
      }

      // Debug functions
      void PrintImports();
  };

};

#endif
