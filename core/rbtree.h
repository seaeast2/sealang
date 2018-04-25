#ifndef _rbtree_h_
#define _rbtree_h_

// Red Black tree
// 1. Every node is either red or black.
// 2. The root is black.
// 3. Every leaf is black.
// 4. If node is red, then both its child are black.
// 5. For each node, all simple path from the node to descendant 
//    leaves contains same number of black node.
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
      nil_ = new RbNode;
      nil_->color_ = BLACK;
    }

    ~RBTree() {
      delete nil_;
    }


    void Insert(RbNode* z) {
      RbNode* y = nil_;
      RbNode* x = root_;
      while (x != nil_) {
        y = x;
        if (z->key_ < x->key_)
          x = x->left_;
        else
          x = x->right_;
      }
      z->parent_ = y;
      if (y == nil_) 
        root_ = z;
      else if (z->key_ < y->key_) 
        y->left_ = z;
      else 
        y->right_ = z;
      z->left_ = nil_;
      z->right_ = nil_;
      z->color_ = RED;
       
      InsertFixup(z);
    }

    void Delete(RbNode* z) {
      RbNode* x, *y;
      RbNode* y = z;
      eColor y_original_color = y->color_;
      if (z->left_ == nil_) { // in case left is blank, take out z and link left to parent.
        x = z->right_;
        Transplant(z, z->right_);
      }
      else if (z->right_ == nil_) { // right child is blank.
        x = z->left_;
        Transplant(z, z->left_)
      }
      else {
        y = TreeMinimum(z->right_);
        y_original_color = y->color_;
        x = y->right_;
        if (y->parent_ == z)
          x->parent_ = y;
        else {
          Transplant(y, y->right_);
          y->right_ = z->right_;
        }
      }
    }


  private:
    // Refer to 'Introduction to Algorithms 3rd Edition : p316-317'
    void InsertFixup(RbNode* z) {
      RbNode* y;
      while(z->parent_->color_ == RED) {
        if (z->parent_ == z->parent_->parent_->left_) {
          y = z->parent_->parent_->right_;
          if (y->color_ == RED) {
            z->parent_->color_ = BLACK;         // case 1
            y->color_ = BLACK;                  // case 1
            z->parent_->parent_->color_ = RED;  // case 1
            z = z->parent_->parent_;            // case 1
          }
          else if (z == z->parent_->right_) {
            z = z->parent_;                     // case 2
            RotateLeft(z);                      // case 2
          }
          else {
            z->parent_->color_ = BLACK;         // case 3
            z->parent_->parent_->color_ = RED;  // case 3
            RotateRight(z->parent_->parent_);   // case 3
          }
        }
        else {
          y = z->parent_->parent_->left_;
          if (y->color_ == RED) {
            z->parent_->color_ = BLACK;         // case 1
            y->color_ = BLACK;                  // case 1
            z->parent_->parent_->color_ = RED;  // case 1
            z = z->parent_->parent_;            // case 1
          }
          else if (z == z->parent_->left_) {
            z = z->parent_;                     // case 2
            RotateRight(z);                     // case 2
          }
          else {
            z->parent_->color_ = BLACK;         // case 3
            z->parent_->parent_->color_ = RED;  // case 3
            RotateLeft(z->parent_->parent_);    // case 3
          }
        }
      }
      root_->color_ = BLACK;
    }

    //      |                      |
    //      y    Left rotation     x
    //     / \   <============    / \
    //    x   c  Right rotation  a   y
    //   / \      ============>     / \
    //  a   b                      b   c
    // 
    void RotateLeft(RbNode* x) {
      RbNode* y = x->right_;  // set y
      x->right_ = y->left_; // turn y's left subtree into x's right subtree.
      if (y->left_ != nil_) 
        y->left_->parent_ = x;

      y->parent_ = x->parent_; // link x's parent to y
      if (x->parent_ == nil_) // check if root
        root_ = y;
      else if (x == x->parent_->left_)
        x->parent_->left_ = y;
      else
        x->parent_->right_ = y;
      y->left_ = x;
      x->parent_ = y;
    }
    void RotateRight(RbNode* y) {
      RbNode* x = y->left_;  // set x
      y->left_ = x->right_; // turn y's left subtree into x's right subtree.
      if (x->right_ != nil_) 
        x->right_->parent_ = y;

      x->parent_ = y->parent_; // link x's parent to y
      if (y->parent_ == nil_) // check if root
        root_ = x;
      else if (y == y->parent_->left_)
        y->parent_->left_ = x;
      else
        y->parent_->right_ = x;
      x->right_ = y;
      y->parent_ = x;
    }

    void Transplant(RbNode* u, RbNode* v) {
      if (u->parent_ == nil_)
        root_ = v;
      else if (u == u->parent_->left_)
        u->parent_->left_ = v;
      else
        u->parent_->right_ = v;
      v->parent_ = u->parent_;
    }

    RbNode* TreeMinimum(RbNode* x) {
      while(x->left_ != nil_) {
        x = x->left_;
      }
      return x;
    }

    RbNode* TreeMaximum(RbNode* x) {
      while(x->right_ != nil_) {
        x = x->right_;
      }
      return x;
    }
};


#endif
