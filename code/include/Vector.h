#pragma once

#include <memory_resource>
#include <array>

namespace nxt {
namespace core {
template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
class Vector {

};
}
}
