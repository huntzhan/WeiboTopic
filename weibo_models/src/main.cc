// ============================================================================
//
//       Filename:  main.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/17/2014 23:58:14
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (zhx), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <iostream>
#include "example/simple_calculator.h"

using std::cout;
using std::endl;
using example::SimpleCalculator;

int main() {
  SimpleCalculator handle;
  cout << handle.SimpleFunc() << endl;
}
