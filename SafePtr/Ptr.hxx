

template <typename T>
Ptr<T>::Ptr(const Ptr& cpy): m_ptr(cpy.m_ptr), toDelete(cpy.toDelete), m_offset(cpy.m_offset) {
  m_id = list[m_ptr].size();
  list[m_ptr].push_back(this);
}

template <typename T>
Ptr<T>::Ptr(const Ptr&& mv): m_ptr(mv.m_ptr), toDelete(mv.toDelete), m_offset(mv.m_offset)  {
  m_id = list[m_ptr].size();
  list[m_ptr].push_back(this);
}

template <typename T>
Ptr<T>::~Ptr() {
  if (newest) {
    newest = false;
  } else {
    pre_dtor();
  }
}

template <typename T>
Ptr<T>& Ptr<T>::operator=(const Ptr &cpy) {
  pre_dtor();
  m_ptr = cpy.m_ptr;
  m_offset = cpy.m_offset;
  m_id = list[m_ptr].size();
  toDelete = cpy.toDelete;
  list[m_ptr].push_back(this);
  return *this;
}

template <typename T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T> &&mv) {
  pre_dtor();
  m_ptr = mv.m_ptr;
  m_offset = mv.m_offset;
  m_id = list[m_ptr].size();
  toDelete = mv.toDelete;
  list[m_ptr].push_back(this);
  return *this;
}

template <typename T>
Ptr<T>& Ptr<T>::operator=(std::nullptr_t null) {
  if (null != nullptr) throw bad_access_ptr();
  pre_dtor();
  m_id = -1;
  m_offset = 0;
  m_ptr = nullptr;
  toDelete = false;
  return *this;
}



template <typename T>
template<typename ...Args>
Ptr<T> Ptr<T>::New(Args... args) {
  auto ptr = new T(args...);
  return Ptr(ptr, true);
}

template <typename T>
Ptr<T> Ptr<T>::Adr(T& variable) {
  return Ptr(&variable, false);
}

template <typename T>
void Ptr<T>::Delete(Ptr& ptr) {
  if (ptr.toDelete) {
    delete ptr.m_ptr;
  }
  for (auto* p : list[ptr.m_ptr]) {
    p->m_ptr = nullptr;
    p->m_id = -1;
    p->toDelete = false;
  }

}


template <typename T>
bool Ptr<T>::operator==(const Ptr& ptr) const {
  return m_ptr == ptr.m_ptr && m_offset == ptr.m_offset;
}

template <typename T>
bool Ptr<T>::operator==(const Ptr&& ptr) const {
  return  m_ptr == ptr.m_ptr && m_offset == ptr.m_offset;
}

template <typename T>
bool Ptr<T>::operator==(const T* ptr) const {
  return m_ptr ? m_ptr + m_offset == ptr : m_ptr == ptr;
}


template <typename T>
Ptr<T> Ptr<T>::operator+(const int i) const {
  if (!m_ptr) throw bad_access_ptr();
  auto&& ret = std::move(*this);
  ret.m_offset += i;
  return ret;
}

template <typename T>
Ptr<T> Ptr<T>::operator-(const int i) const {
  if (!m_ptr) throw bad_access_ptr();
  auto&& ret = std::move(*this);
  ret.m_offset -= i;
  return ret;
}
}

template <typename T>
Ptr<T>& Ptr<T>::operator++() {
  if (!m_ptr) throw bad_access_ptr();
  ++m_offset;
  return *this;
}

template <typename T>
Ptr<T>& Ptr<T>::operator--() {
  if (!m_ptr) throw bad_access_ptr();
  --m_offset;
  return *this;
}


template <typename T>
Ptr<T>::operator bool() const {
  return m_ptr != nullptr;
}

template <typename T>
const T* Ptr<T>::operator->() const {
  if (!m_ptr) throw bad_access_ptr();
    return m_ptr + m_offset;
}

template <typename T>
T* Ptr<T>::operator->() {
  if (!m_ptr) throw bad_access_ptr();
  return m_ptr + m_offset;
}

template <typename T>
const T& Ptr<T>::operator*() const {
  if (!m_ptr) throw bad_access_ptr();
  return *(m_ptr + m_offset);
}

template <typename T>
T& Ptr<T>::operator*() {
  if (!m_ptr) throw bad_access_ptr();
  return  *(m_ptr + m_offset);
}