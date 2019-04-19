#include <array>
#include <iostream>
#include <algorithm>
#include "../include/CompressedPair.h"
#include "../include/List.h"
#include "../include/Search.h"
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

	//std::upper_bound
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

    int test_again[] = {-1, 2, 0, 52, 13, 0, -10, 6};

    nxt::core::partition(std::begin(test_again), std::end(test_again), [](int i) { return i == 0; });

    std::cout << "vector :\n";
    for (auto value : test_again) {
        std::cout << value << '\n';
    }

    std::cout << "search :\n";
    std::cout << *nxt::core::lowerBound(std::begin(test_again), std::end(test_again), 12) << std::endl;
    std::cout << *nxt::core::upperBound(std::begin(test_again), std::end(test_again), 12) << std::endl;
    std::cout << nxt::core::binarySearch(std::begin(test_again), std::end(test_again), 13) << std::endl;
    std::cout << nxt::core::binarySearch(std::begin(test_again), std::end(test_again), 12) << std::endl;

    nxt::core::insertionSort(vector.begin(), vector.end(), std::greater<>());


    std::cout << "vector :\n";
    for (auto value : vector) {
        std::cout << value << '\n';
    }

	std::array merge1 = {1, 3, 5, 7};
    std::array merge2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::array<int, 15> result = {};

	nxt::core::merge(merge1.begin(), merge1.end(), merge2.begin(), merge2.end(), result.begin(), std::less<>());

	std::cout << "Merge Result :\n";
        for (auto value : result) {
            std::cout << value << '\n';
        }

    return 0;
}
