#ifndef _hash_h_
#define _hash_h_

// simple hash table
template <class V, unsigned int MAX_TABLE> 
class HashTable {
  struct Element {
    V* value_;
    Element* next_;
  };

  Element* table_[MAX_TABLE];
  
  public:
    HashTable() {
    }
    ~HashTable() {
    }

    void Insert() {}

    void Find() {}

    void Delete() {}


  private:
    int HashCode(const char* str) {
      int 
    }
};

#endif
