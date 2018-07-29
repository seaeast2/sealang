#ifndef _hash_h_
#define _hash_h_

#include <string>

// string key hash table
template <class V, unsigned int MAX_TABLE> 
class HashTable {
  struct Element {
    char* key_;
    V* value_;
    Element* next_;
    Element* prev_;
  };

  Element* table_[MAX_TABLE];
  
  public:
    HashTable() {
      for (int i = 0; i < MAX_TABLE; i++) {
        table_[i] = nullptr;
      }
    }
    ~HashTable() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = nullptr;
      }
    }

    V* Insert(const char* key, V& value) {
      Element* res = FindElement(key);
      if (!res) {
        // creates new element.
        Element* new_item = new Element;
        new_item->key_ = new char[strlen(key)];
        strcpy(new_item->key_, key);
        new_item->value_ = &value;
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
        return new_item->value_;
      }
      return res->value_;
    }

    V* Find(const char* key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (!strcmp(key, cur->key_)) {
            return cur->value_;
          }
          cur = cur->next_; 
        }
      }
      return nullptr;
    }

    V* Delete(const char* key) {
      Element* res = FindElement(key);
      if (!res)
        return nullptr;

      long hash_code = Hash(key);
      if (res->prev_)
        res->prev_->next_ = res->next_;
      else
        table_[hash_code] = res->next_;

      if (res->next_)
        res->next_->prev_ = res->prev_;

      V* v = res->value_;
      delete[] res->key_;
      delete res;

      return v;
    }

  private:
    // ELF hash function
    // http://www.drdobbs.com/database/hashing-rehashed/184409859
    long Hash(const char* key) {
      long h = 0;
      long g;
      for (;*key;key++) {
        h = (h << 4) + *key;
        g = h & 0xf0000000;
        if (g) 
          h ^= g >> 24;
        h &= ~g;
      }
      return h % MAX_TABLE;
    }

    Element* FindElement(const char* key) {
      long hash_code = Hash(key);
      if (table_[hash_code]) {
        Element* cur = table_[hash_code];
        while(cur) {
          if (!strcmp(key, cur->key_)) {
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

};

#endif
