#ifndef SHARED_PTR
#define SHARED_PTR
template <typename T> class SharedPtr
{
public:
  SharedPtr(T *t)
  {
    new T *ptr = t;
  }
  explicit SharedPtr(const SharedPtr &SP) : SharedPtr(SP){}
  ~SharedPtr();
  SharedPtr &operator=(const SharedPtr &);
  T &        operator*() const;
  T *        operator->() const;
  explicit bool       operator==(const SharedPtr &) const;
  size_t     count() const;
};
#endif