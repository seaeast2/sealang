#ifndef _ast_node_h_
#define _ast_node_h_

namespace AST {
// Abstrat Syntax Tree Base Nodes
  class BaseNode {
    protected:
      BaseNode() {}
    public:
      virtual ~BaseNode() {}

      virtual void print();
  };


  class RootNode : public BaseNode {
    protected:

    public:
      RootNode() {}
      virtual ~RootNode() {}
  };

};
#endif
