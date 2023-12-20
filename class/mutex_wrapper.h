//
//  mutex_wrapper.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef mutex_wrapper_h
#define mutex_wrapper_h

struct mutex_wrapper : mutex
{
  mutex_wrapper() = default;
  mutex_wrapper(mutex_wrapper const&) noexcept : mutex() {}
  bool operator==(mutex_wrapper const&other) noexcept { return this==&other; }
};

#endif /* mutex_wrapper_h */
