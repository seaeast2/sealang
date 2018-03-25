#ifndef _list_h_
#define _list_h_


template <class T> 
class SimpleList {
  public:
  struct Item {
    T value;
    Item* prev_;
    Item* next_;
  };

  private:
    Item* first_;
    Item* last_;
    int count_;

  public:
    SimpleList() { 
      first_ = last_ = NULL; 
      count_ = 0;
    }
    ~SimpleList() {
    }

    Item* AddTail(const T& value) {
      Item* new_item = CreateNewItem(value);

      if (!first_) {
        first_ = last_ = new_item;
      }
      else {
        new_item->prev_ = last_;
        last_->next_ = new_item;
        last_ = new_item;
      }

      count_++;
      return new_item;
    }

    Item* Find(const T& value) {
      Item* itr = first_;
      while(itr->next_) {
        if (itr->value == value)
          return itr;
        itr = itr->next_;
      }
      return NULL;
    }

    Item* GetAt(int idx) {
      if (idx > count_)
        return NULL;

      Item* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++,itr = itr->next_) {
      }

    }

    // insert at index position.
    Item* InsertAt(int idx, const T& value) {
      if (idx > count_)
        return NULL;

      Item* new_item = CreateNewItem(value);

      return NULL;
    }

    Item* Front() {
      return first_;
    }

    Item* Back() {
      return last_;
    }

    int GetSize() {
      return count_;
    }

  private:
    Item* CreateNewItem(const T& value) {
      Item* new_item = new Item;
      new_item->value = value;
      new_item->priv_ = new_item->next_ = NULL;
      return new_item;
    }

};

#endif
