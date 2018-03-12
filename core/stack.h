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
    ~SimpleArrayStack() {}

    void Push(T t) {
      data_[++top_] = t;
    }

    T Pop() {
      if (top_ < 0)
        return data_[0];
      return data_[top_--];
    }

    int Top() { 
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
};

#endif
