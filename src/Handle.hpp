#ifndef Handle_hpp
#define Handle_hpp

#include <memory>

using SharedHandle = std::enable_shared_from_this;

template <class C> class Handle : std::shared_ptr<C>
{

  private:


};


#endif // file
