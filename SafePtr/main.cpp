//
//  main.cpp
//  TestNew
//
//  Created by Florent Carrez on 21/06/2019.
//  Copyright © 2019 Florent.C. All rights reserved.
//

#include <iostream>
#include "Ptr.hpp"

class A {
public:
    virtual void print(int val) {
        std::cout << "i print " << val << '\n';
    }
};


int main(int argc, const char * argv[]) {
    // insert code here...
  auto p = Ptr<int>::New(0);
  
  return 0;
}
