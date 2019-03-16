#pragma once

<<<<<<< HEAD
#include <vector>

std::vector<int> v;

namespace nxt {
namespace core {

template<class T, class Allocator = std::pmr::allocator<T>>
class Vector
{

}
}  // namespace core
}  // namespace nxt
=======
#include <memory_resource>
#include <array>

namespace nxt {
namespace core {
template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
class Vector {

};
}
}
>>>>>>> 2f7a5e1cd766ef7981bf3d613edff460820219dc
