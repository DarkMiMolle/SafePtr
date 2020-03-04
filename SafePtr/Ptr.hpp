#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <exception>

class bad_access_ptr: std::exception {
public:
    const char* what() const noexcept override { return "can't access  to  nullptr";}
};

template <typename T>
class Ptr {
protected:
  bool newest = false;
  bool toDelete = false;
  T* m_ptr = nullptr;
  int m_offset = 0;
  size_t m_id = -1;

    /// \def list is a map of all T* pointers and the associated Ptr to that pointer.
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

  /// \brief make the pointer
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

  Ptr() = default; /// \brief set Ptr as nullptr
  Ptr(const Ptr& copy);
  Ptr(const Ptr&& mv);

  Ptr& operator=(const Ptr& cpy);
  Ptr& operator=(const Ptr&& mv);
  Ptr& operator=(std::nullptr_t null);

  bool operator==(const Ptr& ptr) const;
  bool operator==(const Ptr&& ptr) const;
  bool operator==(const T* ptr) const;

  /// \brief classical + and - operator for pointers
  Ptr operator+(const int i) const ;
  Ptr operator-(const int i) const ;
  Ptr& operator++();
  Ptr& operator--();

  /// \brief return whether or not pointer is nullptr
  operator bool () const;

  const T* operator ->() const;
  T* operator -> ();

  const T& operator * () const;
  T& operator * ();

  /**
   * \brief make a pointer that will be deleted later
   */
  template <typename ...Args>
  static Ptr New(Args... args);

  /// \brief delete ptr if needed and set all Ptr with the same ptr at nullptr
  static void Delete(Ptr& ptr);

  /// \brief build the pointer through a variable. The Ptr won't be deleted but will be set at nullptr as soon as ~Ptr is called
  static Ptr Adr(T& variable);

  /// \brief if the pointer wasn't just created, it will set out the Ptr from the list
  virtual ~Ptr();
};



#include "Ptr.hxx"
