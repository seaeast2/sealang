#ifndef _ast_printer_h_
#define _ast_printer_h_

#include "ast_visitor.h"

namespace AST {
  // Print AST
  class ASTPrinter : VisitorBase<void, void> {
    public:
      ASTPrinter();
      virtual ~ASTPrinter();


  };
}

#endif
