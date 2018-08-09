#ifndef _list_h_
#define _list_h_


// simple linked list
template <class V> 
class SimpleList {
  private:
    struct Element {
      V value;
      Element* prev_;
      Element* next_;
    };
  
    Element* first_;
    Element* last_;
    int count_;

    // Iteration
    Element* cur_;

  public:
    SimpleList() { 
      cur_ = first_ = last_ = nullptr; 
      count_ = 0;
    }
    ~SimpleList() {
      Clear();
    }

    void PushBack(const V& value) {
      Element* new_item = CreateNewItem(value);

      if (first_) {
        new_item->prev_ = last_;
        last_->next_ = new_item;
        last_ = new_item;
      }
      else
        first_ = last_ = new_item;

      count_++;
    }

    bool PopBack() {
      if (count_ == 0)
        return false;

      Element* del_item = last_;
      if (del_item->prev_) {
        del_item->prev_->next_ = nullptr;
        last_ = del_item->prev_;
      }
      else
        first_ = last_ = nullptr;

      count_--;
      delete del_item;
      return true;
    }

    void PushFront(const V& value) {
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
    }

    bool PopFront() {
      if (count_ == 0)
        return false;

      Element* del_item = first_;
      if (del_item->next_) {
        del_item->next_->prev_ = nullptr;
        first_ = del_item->next_;
      }
      else 
        first_ = last_ = nullptr;

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
        count_--;
      }
    }

    bool Delete(const V& value) {
      int idx = FindIndex(value);
      if (idx != -1) {
        return DeleteAt(idx);
      }

      return false;
    }

    V* Find(const V& value) {
      Element* itr = first_;
      while(itr->next_) {
        if (itr->value == value)
          return &itr->value;
        itr = itr->next_;
      }
      return nullptr;
    }

    int FindIndex(const V& value) {
      int idx = 0;
      Element* itr = first_;
      while(itr->next_) {
        if (itr->value == value) {
          return idx;
        }
        idx++;
        itr = itr->next_;
      }
      return -1;
    }

    V* GetAt(int idx) {
      if (idx > count_)
        return nullptr;

      Element* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++, itr = itr->next_) {
      }

      return &itr->value;
    }

    // insert at index position.
    bool InsertAt(int idx, const V& value) {
      if (idx > count_)
        return false;

      Element* new_item;

      Element* idx_item = GetElementAt(idx);
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

      return true;
    }

    bool DeleteAt(int idx) {
      Element* del_item = GetElementAt(idx);
      if (!del_item)
        return false;

      if (del_item->next_) {
        del_item->prev_->next_ = del_item->next_;
        del_item->next_->prev_ = del_item->prev_;
        delete del_item;
        count_--;
      }
      else 
        return PopBack();

      return true;
    }

    V* Front() {
      if (first_)
        return &first_->value;
      return nullptr;
    }

    V* Back() {
      if (last_)
        return &last_->value;
      return nullptr;
    }

    void ItrReset() {
      cur_ = first_;
    }

    V* ItrNext() {
      if (!cur_)
        return nullptr;

      Element* ret = cur_;
      cur_ = cur_->next_;
      return &ret->value;
    }

    int GetSize() {
      return count_;
    }

    V* operator[] (int index) {
      return GetAt(index);
    }

    const V* operator[] (int index) const {
      return GetAt(index);
    }

    SimpleList<V>& operator= (SimpleList<V> const& sv) {
      Clear();
      for (int i = 0; i < sv.GetSize(); i++) {
        PushBack(*sv[i]);
      }

      return *this;
    }

  private:
    Element* CreateNewItem(const V& value) {
      Element* new_item = new Element;
      new_item->value = value;
      new_item->prev_ = new_item->next_ = nullptr;
      return new_item;
    }

    Element* GetElementAt(int idx) {
      if (idx > count_)
        return nullptr;

      Element* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++, itr = itr->next_) {
      }

      return itr;
    }
};

template<class V>
bool operator== (SimpleList<V> const& lhs, SimpleList<V> const& rhs) {
  for (int i = 0; i < lhs.GetSize(); i++) {
    if (*lhs[i] != *rhs[i])
      return false;
  }
  return true;
}

//=================================================================================
// Pointer specialization
template <class V> 
class SimpleList<V*> {
  private:
    struct Element {
      V* value;
      Element* prev_;
      Element* next_;
    };
  
    Element* first_;
    Element* last_;
    int count_;

    // Iteration
    Element* cur_;

  public:
    SimpleList() { 
      cur_ = first_ = last_ = nullptr; 
      count_ = 0;
    }
    ~SimpleList() {
      Clear();
    }

    void PushBack(V* const & value) {
      Element* new_item = CreateNewItem(value);

      if (first_) {
        new_item->prev_ = last_;
        last_->next_ = new_item;
        last_ = new_item;
      }
      else
        first_ = last_ = new_item;

      count_++;
    }

    bool PopBack() {
      if (count_ == 0)
        return false;

      Element* del_item = last_;
      if (del_item->prev_) {
        del_item->prev_->next_ = nullptr;
        last_ = del_item->prev_;
      }
      else
        first_ = last_ = nullptr;

      count_--;
      delete del_item;
      return true;
    }

    void PushFront(V* const & value) {
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
    }

    bool PopFront() {
      if (count_ == 0)
        return false;

      Element* del_item = first_;
      if (del_item->next_) {
        del_item->next_->prev_ = nullptr;
        first_ = del_item->next_;
      }
      else 
        first_ = last_ = nullptr;

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
        count_--;
      }
    }

    bool Delete(V* const& value) {
      int idx = FindIndex(value);
      if (idx != -1) {
        return DeleteAt(idx);
      }

      return false;
    }

    V* Find(V* const& value) {
      Element* itr = first_;
      while(itr->next_) {
        if (itr->value == value)
          return itr->value;
        itr = itr->next_;
      }
      return nullptr;
    }

    int FindIndex(V* const& value) {
      int idx = 0;
      Element* itr = first_;
      while(itr->next_) {
        if (itr->value == value) {
          return idx;
        }
        idx++;
        itr = itr->next_;
      }
      return -1;
    }

    V* GetAt(int idx) {
      if (idx > count_)
        return nullptr;

      Element* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++, itr = itr->next_) {
      }

      return itr->value;
    }

    // insert at index position.
    bool InsertAt(int idx, const V* const& value) {
      if (idx > count_)
        return false;

      Element* new_item;

      Element* idx_item = GetElementAt(idx);
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

      return true;
    }

    bool DeleteAt(int idx) {
      Element* del_item = GetElementAt(idx);
      if (!del_item)
        return false;

      if (del_item->next_) {
        del_item->prev_->next_ = del_item->next_;
        del_item->next_->prev_ = del_item->prev_;
        delete del_item;
        count_--;
      }
      else 
        return PopBack();

      return true;
    }

    V* Front() {
      if (first_)
        return first_->value;
      return nullptr;
    }

    V* Back() {
      if (last_)
        return last_->value;
      return nullptr;
    }

    void ItrReset() {
      cur_ = first_;
    }

    V* ItrNext() {
      if (!cur_)
        return nullptr;

      Element* ret = cur_;
      cur_ = cur_->next_;
      return ret->value;
    }

    int GetSize() {
      return count_;
    }

    V* operator[] (int index) {
      return GetAt(index);
    }

    const V* operator[] (int index) const {
      return GetAt(index);
    }

    SimpleList<V>& operator= (SimpleList<V> const& sv) {
      Clear();
      for (int i = 0; i < sv.GetSize(); i++) {
        PushBack(sv[i]);
      }

      return *this;
    }

  private:
    Element* CreateNewItem(V* const& value) {
      Element* new_item = new Element;
      new_item->value = value;
      new_item->prev_ = new_item->next_ = nullptr;
      return new_item;
    }

    Element* GetElementAt(int idx) {
      if (idx > count_)
        return nullptr;

      Element* itr = first_;
      for (int cnt = 0; cnt < idx; cnt++, itr = itr->next_) {
      }

      return itr;
    }
};

template<class V>
bool operator== (SimpleList<V*> const& lhs, SimpleList<V*> const& rhs) {
  for (int i = 0; i < lhs.GetSize(); i++) {
    if (*lhs[i] != *rhs[i])
      return false;
  }
  return true;
}
#endif
