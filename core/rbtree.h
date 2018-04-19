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

  private:
    RbNode* root_; // root node
    RbNode* nil_; // sentinel leaf node

  public:
    RBTree() {
      root_ = NULL;
      nil_ = NULL;
    }

    ~RBTree() {
    }

    //      |                      |
    //      y    Left rotation     x
    //     / \   <============    / \
    //    x   c  Right rotation  a   y
    //   / \      ============>     / \
    //  a   b                      b   c
    // 
    // Red black tree rotation
    bool RotateLeft(RbNode* x) {
      RbNode* y = x->right_;  // set y
      x->right_ = y->left_; // turn y's left subtree into x's right subtree.
      if (y->left_ != nil_) 
        y->left_->parent_ = x;
      y->parent_ = x->parent_; // link x's parent to y
      if (x->parent_ == nil_) // check root
        root_ = y;
      else if (x == x->parent_->left_)
      
      return true;
    }
};


#endif
