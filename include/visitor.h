#ifndef _visitor_h_
#define _visitor_h_

#include "visitor_base.h"

namespace AST {
  template <class T>
  class Visitor : public VisitorBase<T> {
    public:
      Visitor() {}
      virtual ~Visitor() {}
  };
}

#endif
