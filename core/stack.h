#ifndef _stack_h_
#define _stack_h_


template <class T, unsigned int STACK_SIZE = 100> 
class SimpleArrayStack {
  T data_[STACK_SIZE];

  int top_;

  public:
    SimpleArrayStack() {
      top_ = -1;
    }
    // Copy constructor
    SimpleArrayStack(const SimpleArrayStack &arrstk) {
      for (int i = 0; i < STACK_SIZE; i++) {
        Push(arrstk.GetAt(i));
      }
    }
    ~SimpleArrayStack() {}

    void Push(const T &t) {
      data_[++top_] = t;
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
      top_ = -1;
    }

    // get stack size
    int GetSize() {
      return top_+1;
    }

    int GetMaxStackSize() {
      return STACK_SIZE;
    }

    T& GetAt(unsigned int index) {
      if (index < STACK_SIZE) 
        return data_[index];
    }
};

#endif
