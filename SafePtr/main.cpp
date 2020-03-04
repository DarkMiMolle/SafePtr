//
//  main.cpp
//  TestNew
//
//  Created by Florent Carrez on 21/06/2019.
//  Copyright © 2019 Florent.C. All rights reserved.
//

#include <iostream>
#include "Ptr.hpp"

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
