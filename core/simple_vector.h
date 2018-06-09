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
    SimpleVector();
    SimpleVector(const SimpleVector& sv);
    ~SimpleVector();

    void PushBack(const T& t) {
      if (count_ + 1 > max_size_) {
        T* new_data = new T[max_size_ + DefaultExtendSize];
        max_size_ += DefaultExtendSize;
        // copy items
        Copy(new_data, data_, count_);
        delete[] data_;
        data_ = new_data;
      }

      count_++;
      *(data_ + count_-1) = t;
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
      T* temp; 
      int half = count_/2, back = count_ -1;

      for (int i = 0; i < half; i++) {
        *temp = *(data_ + i);
        *(data_ + i) = *(data_ + back - i);
        *(data_ + back - i) = *temp;
      }
    }

    int GetMaxSize() {
      return max_size_;
    }

    int GetSize() const {
      return count_;
    }

    void Clear() {
      delete[] data_;
      max_size_ = 0;
      count_ = 0;
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

  private:
    void Copy(T* dest, T* src, int size) {
      for (int i = 0; i < size; i++) {
        *(dest+i) = *(src+i);
      }
    }
};

template <class T>
SimpleVector<T>::SimpleVector() {
  count_ = 0;
  max_size_ = DefaultExtendSize;
  data_ = new T[max_size_];
}

template <class T>
SimpleVector<T>::SimpleVector(const SimpleVector& sv) {
  for (int i = 0; i < sv.GetSize(); i++) {
    PushBack(sv[i]);
  }
}

template <class T>
SimpleVector<T>::~SimpleVector() {
  Clear();
}

#endif
