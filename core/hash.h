#ifndef _hash_h_
#define _hash_h_

#include <string.h>

// string key hash table
template <class V, unsigned int MAX_TABLE> 
class HashTable {
  struct Element {
    char* key_;
    V value_;
    Element* next_;
    Element* priv_;
  };

  Element* table_[MAX_TABLE];
  
  public:
    HashTable() {
      for (int i = 0; i < MAX_TABLE; i++) {
        table_[i] = NULL;
      }
    }
    ~HashTable() {
      for (int i = 0; i < MAX_TABLE; i++) {
        if (table_[i]) 
          ClearTable(table_[i]);
        table_[i] = NULL;
      }
    }

    Element* Insert(const char* key, const V& value) {
      Element* res = Find(key);
      if (!res) {
        // creates new element.
        Element* new_item = new Element;
        new_item->key_ = new char[strlen(key)];
        strcpy(new_item->key_, key);
        new_item->V = value;
        new_item->prev_ = NULL;
        new_item->next_ = NULL;

        // empty table
        long hash_code = Hash(key);
        if (!table_[hash_code]) {
          table_[hash_code] = new_item;
          return new_item;
        }

        // insert first.
        table_[hash_code]->priv_ = new_item;
        new_item->next_ = table_[hash_code];
        table_[hash_code] = new_item;
        return new_item;
      }
      return res;
    }

    Element* Find(const char* key) {
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

      return NULL;
    }

    bool Delete(const char* key) {
      Element* res = Find(key);
      if (!res)
        return false;

      long hash_code = Hash(key);
      if (res->priv_)
        res->priv_->next_ = res->next_;
      else
        table_[hash_code] = res->next_;

      if (res->next_)
        res->next_->priv_ = res->priv_;

      delete[] res->key_;
      delete res;

      return true;
    }

  private:
    long Hash(const char* key) {
      long h = 0;
      long g;
      for (;*key;key++) {
        h = (h << 4) + *key;
        g = h & 0xf0000000;
        if (g) 
          h ^= g >> 24;
        h &= g;
      }
      return h % MAX_TABLE;
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
