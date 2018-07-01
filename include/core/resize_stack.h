#ifndef _resize_stack_h_
#define _resize_stack_h_

// resizable array stack 
template <class T> 
class RSStack{
  enum eInnerDefines {
    DefualtExtendSize = 8,
  };

  T* data_;
  int stack_size_;
  int top_;

  public:
  RSStack() {
    data_ = nullptr;
    stack_size_ = 0;
    top_ = -1;
  }

  // copy constructor
  RSStack(const RSStack &st) {
    for (int i = 0; i < st.TopIndex()+1; i++) {
      Push(st.GetAt(i));
    }
  }

  ~RSStack() {
    Clear();
  }

  void Push(const T& t) {
    if (stack_size_ < top_+2) {
      T* new_data = new T[stack_size_ + DefualtExtendSize];
      stack_size_ += DefualtExtendSize;
      // copy items
      Copy(new_data, data_, top_+1);
      delete[] data_;
      data_ = new_data;
    }

    top_++;
    *(data_+top_) = t;
  }

  int Pop() {
    if (top_ < 0)
      return top_;

    return top_--;
  }

  T& Top() { 
    return data_[top_];
  }

  int TopIndex() {
    return top_;
  }
  
  bool IsEmpty() {
    if (top_ < 0)
      return true;
    return false;
  }

  void Clear() {
    delete[] data_;
    stack_size_ = 0;
    top_ = -1;
  }

  void Copy(T* dest, T* src, int size) {
    for (int i = 0; i < size; i++) {
      *(dest+i) = *(src+i);
    }
  }
  
  T& GetAt(unsigned int index) {
    if (index < stack_size_) 
      return data_[index];
    return data_[top_];
  }
};

#endif
