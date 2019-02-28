#include <type_traits>
#include <string>

class A {
public:
    A() = default;
    int a;
};

class B : public A {
public:
    constexpr B() : A() {}
};

void
main() {
    constexpr auto is_nothrow_a = std::is_default_constructible_v<A>;
    constexpr auto is_nothrow_b = std::is_default_constructible_v<B>;
    constexpr B b;
    constexpr auto a = b.a;
    
}
