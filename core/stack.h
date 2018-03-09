#ifndef _stack_h_
#define _stack_h_


template <class T, class S = 100>
class SimpleArrayStack {
  T data_[S];

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
};

#endif