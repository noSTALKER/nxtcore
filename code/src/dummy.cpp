#include "../include/TypeTraits.h"
#include "../include/Array.h"
#include <vector>

void
main() {
    std::vector<int, std::allocator<int>> array;
    constexpr auto value = nxt::core::IsTemplateInstanceOf<std::vector, decltype(array)>::value;
}
