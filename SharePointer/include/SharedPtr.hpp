template <typename T> class SharedPtr
{
public:
  SharedPtr(T *t);
  explicit SharedPtr(const SharedPtr &);
  ~SharedPtr();
  SharedPtr &operator=(const SharedPtr &);
  T &        operator*() const;
  T *        operator->() const;
  bool       operator==(const T &) const;
  size_t     count() const;
};