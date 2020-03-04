#  SafePtr : Ptr<T>
---
Ptr<T> is a _template c++ class_ which facilitate pointer managment.
In add some features like handling loop pointer  to a classical `std::shared_ptr` which make it a little bit less performant.
Let's see below how it works:

---

## Constructors & Detructors

__default constructor__:

It will set the pointer at `nullptr`.

_exemple_:

```c++
void main() {
  Ptr<int> ptr; // ptr is nullptr
}
```

__New__:

The other way to build a Ptr is to use the "New" static method which takes the value of the constructor's type.

This Ptr may be Deleted.

_exemple_:

```c++
void main() {
  auto ptr = Ptr<int>::New(42); // ptr is a new int(42)
}
```

__Adr__:

Adr allows to build a Ptr from an existing variable. Deleted it, will only set all the Ptr which point to the variable address at `nullptr`.

_exemple_:

```c++
void main() {
  int a = 18;
  auto ptr = Ptr<int>::Adr(a); // ptr point to a (ptr == &a)
}
```

__Delete__:

Delete allows to `delete` the pointer if he has been `new` and set all the Ptr which to point the current address at `nullptr`.

_exemple_:

```c++
void main() {
  auto ptr = Ptr<int>::New(31);
  auto ptr2 = ptr;
  Ptr<int>::Delete(ptr); // ptr / ptr2 are deleted AND they are set to nullptr
  int a = 99;
  auto ptr3 = Ptr<int>::Adr(a);
  Ptr<int>::Delete(ptr3); // ptr3 is (only) set to nullptr (as its address wasn't allocate)
}
```

---

## Special method

__LoopOn__:

`LoopOn(Ptr& ptr);` is a method that allows pointer loop. To understand, see the exemple !

_exemple_:

```c++
struct A {
  Ptr<A> next;
};

int main(int argc, const char * argv[]) {
  { // scope
    auto first = Ptr<A>::New(42);
    first->next = Ptr<A>::New();
    first->next->next = first;
  }
  return 0;
}
```

> in that exemple, when we go out of the "scope", w'll have memory leaks because of the pointer loop.
>
> first.next --> @  @.next --> first so it make a loop.

```c++
struct A {
  Ptr<A> next;
};


int main(int argc, const char * argv[]) {
    // insert code here...
  {
    auto first = Ptr<A>::New(42);
    first->next = Ptr<A>::New();
    first->next->next.LoopOn(first);
  }
  return 0;
}
```

> now that we use LoopOn, when first is destroy, @.next will point to nullptr which will avoid all memory leaks in that case since every Ptr will be destroy !

It is possible to make more clean code without using LoopOn by doing:

```c++
struct A {
  Ptr<A> next;
};


int main(int argc, const char * argv[]) {
    // insert code here...
  {
    auto first = Ptr<A>::New(42);
    first->next = Ptr<A>::New();
    first->next->next = first;
    
    Ptr<A>::Delete(first);
  }
  return 0;
}
```

in that case we are 100% sure that they won't have memory leaks :D