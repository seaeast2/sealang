#ifndef _environment_h_
#define _environment_h_

#include "core/hash.h"
#include "core/resize_stack.h"

template <class V>
class Environment {
  HashTable<V, 64> data_;
  RSStack<std::string> key_stack_; // resizable stack

  public:
    Environment() {}
    ~Environment() {
    }

    V* GetValue(const char* key) {
      return data_.Find(key);
    }

    bool Push(char* key, const V& value) {
      if(data_.Find(key))
        return false;

      data_.Insert(key, value);
      key_stack_.Push(key);
      return true;
    }

    bool Pop(const char* key) {
      char* key_top = key_stack_.Top();
      if (!strcmp(key_top, key)) {
        key_stack_.Pop();
        data_.Delete(key);
        return true;
      }
      return false;
    }
};

#endif
