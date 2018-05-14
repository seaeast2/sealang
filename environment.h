#ifndef _environment_h_
#define _environment_h_

#include "core/hash.h"
#include "core/stack.h"

template <class K, class V>
class Environment {
  HashTable<V> data_;
  RSStack<K> key_stack_;

  public:
    Environment() {}
    ~Environment() {}

    V* Get(const K & key) {
      // <<== working
      return NULL;
    }

};

#endif
