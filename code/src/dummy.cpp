#include <array>
#include <iostream>
#include "../include/CompressedPair.h"
#include "../include/List.h"
#include "../include/Sort.h"
#include "../include/Vector.h"

int
main() {
    nxt::core::List<int> list(5, 5);
    list.emplaceBack(1);
    list.emplaceBack(2);
    list.emplaceBack(3);

    list.insert(list.begin(), 0);
    list.insert(list.end(), 4);

    list.popBack();
    list.popFront();

    list.pushBack(10);
    list.pushFront(-10);

    std::array test = {20, 20, 20};

    constexpr auto value = nxt::core::IsIteratorV<decltype(test.begin())>;
    constexpr auto test2 = nxt::core::IsForwardIteratorV<int*>;
    constexpr auto test3 = nxt::core::IsForwardIteratorV<int>;
    constexpr auto test4 = nxt::core::IsForwardIteratorV<decltype(list.begin())>;
    list.insert(list.end(), {20, 30, 40});

    for (auto value : list) {
        std::cout << value << '\n';
    }

    nxt::core::List<int> vector;
    vector.pushBack(3);
    vector.pushBack(1);
    vector.pushBack(2);
    vector.pushBack(4);
    vector.pushBack(0);

    int test_again[] = {-1, 2, 52, 13, 0, -10, 6};

    nxt::core::insertionSort(std::begin(test_again), std::end(test_again));

    std::cout << "vector :\n";
    for (auto value : test_again) {
        std::cout << value << '\n';
    }

    nxt::core::insertionSort(vector.begin(), vector.end(), std::greater<>());

    std::cout << "vector :\n";
    for (auto value : vector) {
        std::cout << value << '\n';
    }

    return 0;
}
