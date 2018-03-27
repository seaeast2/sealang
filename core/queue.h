#ifndef _queue_h_
#define _queue_h_


template <class T, unsigned int QUEUE_SIZE = 100> 
class SimpleQueue{
  T data_[QUEUE_SIZE];

  int front_, back_;
  int count_;

  public:
    SimpleQueue() {
      front_ = 0;
      back_ = -1;
    }
    // Copy constructor
    SimpleQueue(const SimpleQueue &queue) {
      for (int i = 0; i < QUEUE_SIZE; i++) {
        Push(queue.GetAt(i));
      }
    }
    ~SimpleQueue() {}

    int PushBack(const T &t) {
      int inc_back = IncreaseNum(back_);
      if (front_ == inc_back)
        return -1; // Queue is full.

      data_[inc_back]= t;
      
      count_++;
      back_ = inc_back;
      return back_;
    }

    int PopFront() {
      int inc_front = IncreaseNum(front_);
      if (count_ == 0)
        return -1; // already empty.

      count_--;
      front_ = inc_front;
      return front_;
    }

    T& Front() {
      return data_[front_];
    }

    T& Back() {
      return data_[back_];
    }

    int IncreaseNum(int num) {
      return (num+1)%QUEUE_SIZE;
    }

    bool IsEmpty() {
      return count_;
    }

    void Clear() {
      count_ = 0;
      front_ = 0;
      back_ = -1;
    }

    int GetMaxQueueSize() {
      return QUEUE_SIZE;
    }
};



#endif
