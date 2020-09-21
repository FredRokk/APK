#ifndef SHAREDPTR
#define SHAREDPTR
template <typename T> class SharedPtr
{
public:
  explicit SharedPtr(T *t = nullptr)
  {
    ptr_ = t;
    new count_ = count();
    if (ptr_)
    {
      count_++;
    }
  };
  SharedPtr(const SharedPtr<T> &SP) : SharedPtr(SP){}
  ~SharedPtr()
  {
    count_--;
    if(count())
    {
     return; 
    }
    delete(ptr_);
  }
  SharedPtr &operator=(const SharedPtr &newPoint)
  {
    ptr_ = newPoint;
    return *this;
  }
  T &        operator*() const
  {
    return *ptr_;
  }
  T *        operator->() const
  {
    return ptr_;
  }
  /* bool       operator==(const SharedPtr &otherPtr) const
  {
    if (ptr_* == otherPtr*)
    {
      return true;
    }
    else
    {
      return false;
    }    
  } */
  size_t     count() const
  {
    return count_;
  }

private:
  T *ptr_;
  size_t count_ = 0;
};
#endif