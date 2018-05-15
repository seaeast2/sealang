#ifndef _environment_h_
#define _environment_h_

#include "core/hash.h"
#include "core/stack.h"

template <class V>
class Environment {
  HashTable<V, 64> data_;
  RSStack<char*> key_stack_; // resizable stack

  public:
    Environment() {}
    ~Environment() {
      Clear();
    }

    V* Get(const char* key) {
      return data_.Find(key);
    }

    bool Push(const char* key, const V& value) {
      if(data_.Find(key))
        return false;

      data_.Insert(key, value);
      key_stack_.Push(key);
      return true;
    }

    bool Pop(const char* key) {
      char* key_top = key_stack_.Top();
      if (key_top == key)
    }
};

#endif
