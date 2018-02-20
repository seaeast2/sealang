#ifndef _ast_h_
#define _ast_h_

namespace AST {
// Abstrat Syntax Tree Base Nodes
  class Node {
    protected:
      Node() {}
    public:
      virtual ~Node() {}

      virtual void print();
  };

};
#endif
