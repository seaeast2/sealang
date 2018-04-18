#ifndef _btree_h_
#define _btree_h_

// binary tree
template <class K, class V> 
class BTree {
  public:
  struct BtNode {
    K key_;
    V value_;
    BtNode* parent_, *left_, *right_;
  };

  private:
  BtNode* root_;

  public:
    BTree() {
      root_ = NULL;
    }

    ~BTree() {
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


    BtNode* FindNode(const K& key) {
      BtNode* focus_node = root_;

      while(focus_node->key_ != key) {
        if (key < focus_node->key) {
          focus_node = focus_node->left_;
        }
        else {
          focus_node = focus_node->right_;
        }
      }

      return focus_node;
    }

    /*BtItr* Begin();
    BtItr* End();*/
};

#endif
