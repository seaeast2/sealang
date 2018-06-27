#ifndef _ast_printer_h_
#define _ast_printer_h_

#include "visitor.h"
#include "ast_node.h"

namespace AST {

  // Print AST
  class ASTPrinter : public Visitor<void> {
    public:
      ASTPrinter();
      virtual ~ASTPrinter();

      void Print(Declarations* decls);
      void Print(BaseNode* ast);
  };
}

#endif
