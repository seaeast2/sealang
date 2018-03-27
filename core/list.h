#ifndef _list_h_
#define _list_h_


template <class T> 
class SimpleList {
  public:
  struct Element {
    T value;
    Element* prev_;
    Element* next_;
  };

  private:
    Element* first_;
    Element* last_;
    int count_;

  public:
    SimpleList() { 
      first_ = last_ = NULL; 
      count_ = 0;
    }
    ~SimpleList() {
      Clear();
    }

    Element* PushBack(const T& value) {
      Element* new_item = CreateNewItem(value);

      if (first_) {
        new_item->prev_ = last_;
        last_->next_ = new_item;
        last_ = new_item;
      }
      else
        first_ = last_ = new_item;

      count_++;
      return new_item;
    }

    bool PopBack() {
      if (count_ == 0)
        return false;

      Element* del_item = last_;
      if (del_item->prev_) {
        del_item->prev_->next_ = NULL;
        last_ = del_item->prev_;
      }
      else
        first_ = last_ = NULL;

      count_--;
      delete del_item;
      return true;
    }

    Element* PushFront(const T& value) {
      Element* new_item = CreateNewItem(value);

      if (first_) {
        first_->prev_ = new_item;
        new_item->next_ = first_;
        first_ = new_item;
      }
      else {
        first_ = last_ = new_item;
      }

      count_++;
      return new_item;
    }

    bool PopFront() {
      if (count_ == 0)
        return false;

      Element* del_item = first_;
      if (del_item->next_) {
        del_item->next_->prev_ = NULL;
        first_ = del_item->next_;
      }
      else 
        first_ = last_ = NULL;

      count_--;
      delete del_item;
      return true;
    }

    void Clear() {
      Element* itr = first_;
      while(itr) {
        Element* del_item = itr;
        itr = itr->next_;

        delete del_item;
        count--;
      }
    }

    Element* Find(const T& value) {
      Element* itr = first_;
      while(itr->next_) {
        if (itr->value == value)
          return itr;
        itr = itr->next_;
      }
      return NULL;
    }

    Element* GetAt(int idx) {
      if (idx > count_)
        return NULL;

      Element* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++, itr = itr->next_) {
      }

      return itr;
    }

    // insert at index position.
    Element* InsertAt(int idx, const T& value) {
      if (idx > count_)
        return NULL;

      Element* new_item;

      Element* idx_item = GetAt(idx);
      if (idx_item->prev_) {
        new_item = CreateNewItem(value);
        new_item->next_ = idx_item;
        new_item->prev_ = idx_item->prev_;
        idx_item->prev_->next_ = new_item;
        idx_item->prev_ = new_item;
        count_++;
      }
      else {
        new_item = PushFront(value);
      }

      return new_item;
    }

    bool DeleteAt(int idx) {
      Element* del_item = GetAt(idx);
      if (!del_item)
        return false;

      if (del_item->next_) {
        del_item->prev_->next_ = del_item->next_;
        del_item->next_->prev_ = del_item->prev_;
        delete del_item;
        count_--;
      }
      else {
        PopBack();
      }

      return true;
    }

    Element* Front() {
      return first_;
    }

    Element* Back() {
      return last_;
    }

    int GetSize() {
      return count_;
    }

  private:
    Element* CreateNewItem(const T& value) {
      Element* new_item = new Element;
      new_item->value = value;
      new_item->priv_ = new_item->next_ = NULL;
      return new_item;
    }

};

#endif
