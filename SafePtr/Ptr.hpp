#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>

template <typename T>
class Ptr {
protected:
  bool newest = false;
  bool toDelete = false;
  T* m_ptr = nullptr;
  size_t m_id = -1;

  inline static std::map<T*, std::vector<Ptr*>> list;

  void pre_dtor() {
    if (m_ptr) {
      if (list[m_ptr].size() > 1) {
        list[m_ptr][m_id] = list[m_ptr].back();
        list[m_ptr].pop_back();
      } else {
        list.erase(m_ptr);
        if (toDelete) {
          delete m_ptr;
        }
      }
    }
  }

  Ptr(T* ptr, bool del): m_ptr(ptr), toDelete(del) {
    newest = true;
    if (!list.count(ptr)) {
      list[ptr] = {this};
    } else {
      list[ptr].push_back(this);
    }
    m_id = list[ptr].size() - 1;
  }
public:

  Ptr() = default;
  Ptr(const Ptr& copy);
  Ptr(const Ptr&& mv);

  Ptr& operator=(const Ptr& cpy);
  Ptr& operator=(const Ptr&& mv);
  Ptr& operator=(std::nullptr_t null);

  bool operator==(const Ptr& ptr) const;
  bool operator==(const Ptr&& ptr) const;
  bool operator==(const T* ptr) const;

  Ptr& operator+(const int i);
  Ptr& operator-(const int i);

  operator bool () const;

  const T* operator ->() const;
  T* operator -> ();

  const T& operator * () const;
  T& operator * ();

  /**
   * \brief make a pointer that will be deleted
   */
  template <typename ...Args>
  static Ptr New(Args... args);

  /// \brief delete ptr if needed and set all Ptr with the same ptr at nullptr
  static void Delete(Ptr& ptr);

  /// \brief build the pointer through a variable. The Ptr won't be deleted but will be set at null as soon as ~Ptr is called
  static Ptr Adr(T& variable);

  virtual ~Ptr();
};

template <typename T>
Ptr<T>::Ptr(const Ptr& cpy) {
  m_ptr = cpy.m_ptr;
  toDelete = cpy.toDelete;
  m_id = list[m_ptr].size();
  list[m_ptr].push_back(this);
}

template <typename T>
Ptr<T>::Ptr(const Ptr&& mv) {
  m_ptr = mv.m_ptr;
  toDelete = mv.toDelete;
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
  m_id = list[m_ptr].size();
  toDelete = cpy.toDelete;
  list[m_ptr].push_back(this);
  return *this;
}

template <typename T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T> &&mv) {
  pre_dtor();
  m_ptr = mv.m_ptr;
  m_id = list[m_ptr].size();
  toDelete = mv.toDelete;
  list[m_ptr].push_back(this);
  return *this;
}

template <typename T>
Ptr<T>& Ptr<T>::operator=(std::nullptr_t null) {
  if (null != nullptr) throw "TODO: throw bad adresse exception";
  pre_dtor();
  m_id = -1;
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

#include "Ptr.hxx"
