#ifndef _ast_printer_h_
#define _ast_printer_h_

#include "ast_visitor.h"

namespace AST {

  // Print AST
  class ASTPrinter : VisitorBase<void> {
    public:
      ASTPrinter();
      virtual ~ASTPrinter();
  };


  class Test : VisitorBase<int> {
    public:
      Test() {}
      virtual ~Test() {}
  };
}

#endif
