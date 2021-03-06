#ifndef _hash_int_h_
#define _hash_int_h_

using namespace std;

// integer key hash table
template <class K, class V, int MAX_TABLE>
class HashInt {
  struct Element {
    K key_;
    V value_;
    Element* next_;
    Element* prev_;
  };

  Element* table_[MAX_TABLE];
  int count_;

  // hash iterator
  int table_iter_;
  Element* itr_;
  
  public:
    HashInt() {
      for (int i = 0; i < MAX_TABLE; i++) {
        table_[i] = nullptr;
      }
      count_ = 0;
    }
    ~HashInt() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = nullptr;
      }
      count_ = 0;
    }

    V* Insert(K key, const V& value) {
      Element* res = FindElement(key);
      if (!res) {
        // creates new element.
        Element* new_item = new Element;
        new_item->key_ = key;
        new_item->value_ = value;
        new_item->prev_ = nullptr;
        new_item->next_ = nullptr;

        // empty table
        long hash_code = Hash(key);
        if (!table_[hash_code]) {
          table_[hash_code] = new_item;
          return &new_item->value_;
        }

        // insert first.
        table_[hash_code]->prev_ = new_item;
        new_item->next_ = table_[hash_code];
        table_[hash_code] = new_item;
        
        count_++;
        return &new_item->value_;
      }
      return &res->value_;
    }

    V* Find(K key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (key == cur->key_) {
            return &cur->value_;
          }
          cur = cur->next_; 
        }
      }
      return nullptr;
    }

    bool Delete(K key) {
      Element* res = FindElement(key);
      if (!res)
        return false;

      long hash_code = Hash(key);
      if (res->prev_)
        res->prev_->next_ = res->next_;
      else
        table_[hash_code] = res->next_;

      if (res->next_)
        res->next_->prev_ = res->prev_;

      delete[] res->key_;
      //delete res->value_;
      delete res;
      count_--;

      return true;
    }

    int GetSize() { return count_; }

    // value order is not guaranteed.
    V* GetValue(int index) { 
      if (index >= count_)
        return nullptr;

      int found_count_ = 0;
      // table iteration
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) {
          Element* e = table_[i];
          while(e) {
            found_count_++;
            if (found_count_-1 == index)
              return &e->value_;
            e = e->next_;
          }
        }
      }

      return nullptr;
    }

    void ResetItr() { 
      // find first element
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) {
          table_iter_ = i;
          itr_ = table_[i];
          break;
        }
      }
    }
    
    V* Next() {
      if (table_iter_ == MAX_TABLE)
        return nullptr;

      if (!itr_) {
        for(int i = table_iter_+1; i < MAX_TABLE; i++) {
          if (table_[i]) {
            table_iter_ = i;
            itr_ = table_[i];
            break;
          }
        }
      }

      Element* cur = itr_;
      itr_ = itr_->next_;
      return &cur->value_;
    }

    void Clear() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = nullptr;
      }
      count_ = 0;
    }

  private:
    // 64bit hash function
    unsigned long Hash(K key) {
      unsigned long v = key * 3935559000370003845 + 2691343689449507681;

      v ^= v >> 21;
      v ^= v << 37;
      v ^= v >>  4;

      v *= 4768777513237032717;

      v ^= v << 20;
      v ^= v >> 41;
      v ^= v <<  5;

      return v % MAX_TABLE;
    }

    Element* FindElement(K key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (key == cur->key_) {
            return cur;
          }
          cur = cur->next_; 
        }
      }
      return nullptr;
    }

    void ClearTable(Element* e) {
      Element* cur = e, *next;
      while(cur) {
        next = cur->next_;
        delete cur;
        cur = next;
      }
    }

    void DestroyTable(Element* e) {
      Element* cur = e, *next;
      while(cur) {
        next = cur->next_;
        delete cur;
        cur = next;
      }
    }
};

// Pointer specialization ===============================================================
template <class K, class V, int MAX_TABLE> 
class HashInt<K, V*, MAX_TABLE> {
  struct Element {
    K key_;
    V* value_;
    Element* next_;
    Element* prev_;
  };

  Element* table_[MAX_TABLE];
  int count_;

  // hash iterator
  int table_iter_;
  Element* itr_;
  
  public:
    HashInt() {
      for (int i = 0; i < MAX_TABLE; i++) {
        table_[i] = nullptr;
      }
      count_ = 0;
    }
    ~HashInt() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = nullptr;
      }
      count_ = 0;
    }

    V* Insert(K key, V* const& value) {
      Element* res = FindElement(key);
      if (!res) {
        // creates new element.
        Element* new_item = new Element;
        new_item->key_ = key;
        new_item->value_ = value;
        new_item->prev_ = nullptr;
        new_item->next_ = nullptr;

        // empty table
        long hash_code = Hash(key);
        if (!table_[hash_code]) {
          table_[hash_code] = new_item;
          return new_item->value_;
        }

        // insert first.
        table_[hash_code]->prev_ = new_item;
        new_item->next_ = table_[hash_code];
        table_[hash_code] = new_item;
        count_++;
        return new_item->value_;
      }
      return res->value_;
    }

    V* Find(K key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (key == cur->key_) {
            return cur->value_;
          }
          cur = cur->next_; 
        }
      }
      return nullptr;
    }

    bool Delete(K key) {
      Element* res = FindElement(key);
      if (!res)
        return false;

      long hash_code = Hash(key);
      if (res->prev_)
        res->prev_->next_ = res->next_;
      else
        table_[hash_code] = res->next_;

      if (res->next_)
        res->next_->prev_ = res->prev_;

      delete[] res->key_;
      delete res->value_;
      delete res;
      count_--;

      return true;
    }

    int GetSize() { return count_; }

    // value order is not guaranteed.
    V* GetValue(int index) { 
      if (index >= count_)
        return nullptr;

      int found_count_ = 0;
      // table iteration
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) {
          Element* e = table_[i];
          while(e) {
            found_count_++;
            if (found_count_-1 == index)
              return e->value_;
            e = e->next_;
          }
        }
      }

      return nullptr;
    }

    void ResetItr() { 
      // find first element
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) {
          table_iter_ = i;
          itr_ = table_[i];
          break;
        }
      }
    }
    
    V* Next() {
      if (table_iter_ == MAX_TABLE)
        return nullptr;

      if (!itr_) {
        for(int i = table_iter_+1; i < MAX_TABLE; i++) {
          if (table_[i]) {
            table_iter_ = i;
            itr_ = table_[i];
            break;
          }
        }
      }

      Element* cur = itr_;
      itr_ = itr_->next_;
      return cur->value_;
    }

    void Clear() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = nullptr;
      }
      count_ = 0;
    }

  private:
    // 64bit hash function
    unsigned long Hash(K key) {
      unsigned long v = key * 3935559000370003845 + 2691343689449507681;

      v ^= v >> 21;
      v ^= v << 37;
      v ^= v >>  4;

      v *= 4768777513237032717;

      v ^= v << 20;
      v ^= v >> 41;
      v ^= v <<  5;

      return v % MAX_TABLE;
    }

    Element* FindElement(K key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (key == cur->key_) {
            return cur;
          }
          cur = cur->next_; 
        }
      }
      return nullptr;
    }

    void ClearTable(Element* e) {
      Element* cur = e, *next;
      while(cur) {
        next = cur->next_;
        delete cur->value_;
        delete cur;
        cur = next;
      }
    }
};

#endif
