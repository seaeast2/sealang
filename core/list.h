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
  struct Data {
    Item* first_;
    Item* last_;
  };
  Data* data_;

  public:
    SimpleList() { data_ = NULL; }
    ~SimpleList() {}

    void Add(const T& value) {
      if (data_ == NULL) {
        data_ = new Data;
      }
    }

};

#endif
