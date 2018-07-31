#ifndef _simple_vector_h_
#define _simple_vector_h_


// resizable simple vector
template <class T>
class SimpleVector {
  enum eInnerDefines {
    DefaultExtendSize = 8, 
  };

  T* data_;
  int max_size_;
  int count_; // item count 

  public:
    SimpleVector() {
      count_ = 0;
      max_size_ = DefaultExtendSize;
      data_ = new T[max_size_];
    }

    SimpleVector(SimpleVector<T> const& sv) {
      if (data_)
        delete[] data_;

      max_size_ = sv.GetMaxSize();
      count_ = sv.GetSize();

      for (int i = 0; i < count_; i++) {
        data_[i] = sv.data_[i];
      }
    }

    ~SimpleVector() {
      delete[] data_;
      max_size_ = 0;
      count_ = 0;
    }

    void PushBack(const T& t) {
      if (count_ + 1 > max_size_) {
        max_size_ += DefaultExtendSize;
        T* new_data = new T[max_size_];

        for (int i = 0; i < max_size_; i++) {
          new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
      }

      data_[count_++] = t;
    }

    const T& PopBack() {
      T* t = (data_ + count_-1); 
      count_--;
      return *t;
    }

    const T& Front() {
      return *data_;
    }

    const T& Back() {
      return *(data_ + count_ -1);
    }

    void Reverse() {
      T temp; 
      int half = count_/2, back = count_ -1;

      for (int i = 0; i < half; i++) {
        temp = *(data_ + i);
        *(data_ + i) = *(data_ + back - i);
        *(data_ + back - i) = temp;
      }
    }

    int GetMaxSize() const {
      return max_size_;
    }

    int GetSize() const {
      return count_;
    }

    bool IsEmpty() {
      if (count_ > 0) 
        return false;
      return true;
    }

    T& operator[] (int index) {
      if (index > count_-1)
        return *(data_ + count_-1);
      if (index < 0)
        return *(data_);
      return *(data_ + index);
    }

    const T& operator[] (int index) const {
      if (index > count_-1)
        return *(data_ + count_-1);
      if (index < 0)
        return *(data_);
      return *(data_ + index);
    }

    SimpleVector<T>& operator= (SimpleVector<T> const& sv) {
      if (data_)
        delete[] data_;

      max_size_ = sv.GetMaxSize();
      count_ = sv.GetSize();

      for (int i = 0; i < count_; i++) {
        data_[i] = sv.data_[i];
      }

      return *this;
    }
};

template<typename T>
bool operator== (SimpleVector<T> const& lhs, SimpleVector<T> const& rhs) {
  for (int i = 0; i < lhs.GetSize(); i++) {
    if (*lhs[i] != *rhs[i])
      return false;
  }
  return true;
}

#endif
