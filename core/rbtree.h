#ifndef _rbtree_h_
#define _rbtree_h_

// Red Black tree
// 1. Every node is either red or black.
// 2. The root is black.
// 3. Every leaf is black.
// 4. If node is red, then both its child are black.
// 6. For each node, all simple path from the node to descendant leaves contains same number of black node.
//
template <class K, class V> 
class RBTree {
  public:
    enum eColor {
      RED = 0,
      BLACK, 
    };
    struct RbNode {
      K key_;
      V value_;
      eColor color_;
      RbNode* parent_, *left_, *right_;
    };
  public:
    RBTree() {
    }

    ~RBTree() {
    }
};


#endif
