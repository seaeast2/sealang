#ifndef _btree_h_
#define _btree_h_

// binary tree
template <class K, class V> 
class BTree {
  struct Node {
    K key_;
    V value_;
    Node* left_, *right_;
  };

  Node* root_;

  public:
    BTree() {
      root_ = NULL;
    }

    ~BTree() {
    }

    // clear();
};

#endif
