#ifndef _btree_h_
#define _btree_h_

// binary search tree
template <class K, class V> 
class BSTree {
  public:
  struct BtNode {
    K key_;
    V value_;
    BtNode* parent_, *left_, *right_;
  };

  private:
  BtNode* root_;

  public:
    BSTree() {
      root_ = NULL;
    }

    ~BSTree() {
      Clear();
    }

    bool AddNode(const K& key, const V& value) {
      BtNode* newnode = new BtNode;
      newnode->key_ = key;
      newnode->value_ = value;
      newnode->parent_ = NULL;
      newnode->left_ = NULL;
      newnode->right_ = NULL;

      if (root_ == NULL) {
        root_ = newnode;
      }
      else {
        BtNode* focus_node = root_;
        BtNode* parent = NULL;
        while(true) {
          parent = focus_node;
          if (key < focus_node->key) {
            focus_node = focus_node->left_;
            if (focus_node == NULL) {
              parent->left_ = newnode;
              newnode->parent_ = parent;
              return true;
            }
          }
          else if (key == focus_node->key_) {
            // duplicate key. 
            return false;
          }
          else {
            focus_node = focus_node->right_;
            if (focus_node == NULL) {
              parent->right_= newnode;
              newnode->parent_ = parent;
              return true;
            }
          }
        }
      }
      assert("Binary Tree Node adding error");
      return false;
    }

    bool DelNode(const K& key) {
      BtNode* del_node = FindNode(key);
      if(del_node == NULL) 
        return false;

      // case 1 : no child node
      if (del_node->left_ == NULL && del_node->right_ == NULL) {
        // check if del_node is left or right child.
        if (del_node == del_node->parent_->left_) {
          del_node->parent_->left_ = NULL;
        }
        else {
          del_node->parent_->right_ = NULL;
        }
        delete del_node;
        return true;
      }

      // case 2 : one child
      if (del_node->left_ == NULL && del_node->right_ != NULL) {
        // check if del_node is left or right child.
        if (del_node == del_node->parent_->left_)
          del_node->parent_->left_ = del_node->right_;
        else
          del_node->parent_->right_ = del_node->right_;
        del_node->right_->parent_ = del_node->parent_;
        delete del_node;
        return true;
      }
      else if (del_node->left_ != NULL && del_ndoe->right_ == NULL) {
        // check if del_node is left or right child.
        if (del_node == del_node->parent_->left_)
          del_node->parent_->left_ = del_node->left_;
        else
          del_node->parent_->right_ = del_node->left_;
        del_node->left_->parent_ = del_node->parent_;
        delete del_node;
        return true;
      }

      // case 3 : two child
      if (del_node->left_ != NULL && del_node->right_ != NULL) {
        // find min node from right child
        BtNode* right_min_node = FindMinNode(del_node->right_);
        if (right_min_node == NULL)
          return false;
        // check if right_min_node have right child
        if (right_min_node->right_ != NULL) {
          right_min_node->parent_->left_ = right_min_node->right_;
          right_min_node->right_->parent_ = right_min_node->parent_;
        }
        else {
          right_min_node->parent_->left_ = NULL;
        }
        // replace del_node with right_min_node
        right_min_node->left_ = del_node->left_;
        right_min_node->right_ = del_node->right_;
        right_min_node->parent_ = del_node->parent_;
        del_node->left_->parent_ = right_min_node;
        del_node->right_->parent_ = right_min_node;

        // check if del_node is left or right child.
        if (del_node == del_node->parent_->left_) {
          del_node->parent_->left_ = right_min_node;
        }
        else {
          del_node->parent_->right_ = right_min_node;
        }
        delete del_node;
        return true;
      }

      return false;
    }

    // find smallest node in given tree
    BtNode* FindMinNode(const BtNode* top_node) {
      BtNode* focus_node = top_node;
      while(focus_node->left_ != NULL) {
        focus_node = focus_node->left_;
      }
      return focus_node;
    }

    // find biggest node in given tree
    BtNode* FindMaxNode(const BtNode* top_node) {
      BtNode* focus_node = top_node;
      while(focus_node->right_ != NULL) {
        focus_node = focus_node->right_;
      }
      return focus_node;
    }

    BtNode* FindNode(const K& key) {
      BtNode* focus_node = root_;

      while(focus_node->key_ != key) {
        if (key < focus_node->key) {
          focus_node = focus_node->left_;
        }
        else {
          focus_node = focus_node->right_;
        }
        if (focus_node == NULL)
          return NULL;
      }
      return focus_node;
    }

    void Clear(BtNode* node) {
      BtNode* bkleft, *bkright;
      bkleft = node->left_;
      bkright = node->right_;
      delete node;

      if (bkleft != NULL)
        Clear(bkleft);
      if (bkright != NULL)
        Clear(bkright);
    }
};

#endif
