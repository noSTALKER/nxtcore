#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include "../include/Array.h"
#include "../include/CompressedPair.h"
#include "../include/List.h"
#include "../include/Math.h"
#include "../include/PageVector.h"
#include "../include/PriorityQueue.h"
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

    nxt::core::Array<int, 3> test = {20, 20, 20};

    nxt::core::List testing_list(test.begin(), test.end());

    constexpr auto value = nxt::core::IsIteratorV<decltype(test.begin())>;
    constexpr auto test2 = nxt::core::IsForwardIteratorV<int*>;
    constexpr auto test3 = nxt::core::IsForwardIteratorV<int>;
    constexpr auto test4 = nxt::core::IsForwardIteratorV<decltype(list.begin())>;

    constexpr auto is_4 = nxt::core::isPowerOf2(4u);
    constexpr auto is_odd = nxt::core::isEven(3u);

    // std::upper_bound
    list.insert(list.end(), {20, 30, 40});

    for (auto value : list) {
        std::cout << value << '\n';
    }

    list.reverse();
    list.assign(test.begin(), test.end());
    list.resize(10, 1);
    list.resize(5);

    std::cout << "List after reversing :\n";

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

    std::cout << std::boolalpha << "Before Sort: " << nxt::core::isSorted(std::begin(test_again), std::end(test_again))
              << std::endl;

    nxt::core::insertionSort(std::begin(test_again), std::end(test_again));

    std::cout << std::boolalpha << "After Sort: " << nxt::core::isSorted(std::begin(test_again), std::end(test_again))
              << std::endl;

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

    nxt::core::PageVector<int, 32> page_vec;
    for (int i = 0; i < 100; ++i) {
        page_vec.pushBack(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(page_vec.begin(), page_vec.end(), g);

    std::cout << "Page Vector:\n";

    nxt::core::partialSort(page_vec.begin(), page_vec.begin() + 21, page_vec.end());

    std::cout << "Page Vector after sort:\n";

    for (auto value : page_vec) {
        std::cout << value << '\n';
    }

    std::cout << "Is Sorted : " << nxt::core::isSorted(page_vec.begin(), page_vec.end()) << '\n';

    nxt::core::PageVector<int, 32> page_vec_heap;
    page_vec_heap.pushBack(0);
    page_vec_heap.pushBack(7);
    page_vec_heap.pushBack(1);
    page_vec_heap.pushBack(5);
    page_vec_heap.pushBack(3);
    page_vec_heap.pushBack(7);
    page_vec_heap.pushBack(4);
    page_vec_heap.pushBack(4);
    page_vec_heap.pushBack(2);
    page_vec_heap.pushBack(0);
    page_vec_heap.pushBack(6);

    nxt::core::makeHeap(page_vec_heap.begin(), page_vec_heap.end());

    std::cout << "Page Vector after heapify:\n";

    for (auto value : page_vec_heap) {
        std::cout << value << '\n';
    }

    std::cout << "Is Heap : " << nxt::core::isHeap(page_vec_heap.begin(), page_vec_heap.end()) << '\n';

    nxt::core::PriorityQueue<int> queue;
    queue.push(2);
    queue.push(4);
    queue.push(0);
    queue.push(7);
    std::cout << "Queue empty : " << queue.empty() << '\n';
    std::cout << "Queue size : " << queue.size() << '\n';

    while (!queue.empty()) {
        std::cout << queue.popAndExtract() << '\n';
    }

    nxt::core::Vector count_vec(10, 1);

    std::cout << "Count Vector:" << '\n';

    for (auto value : count_vec) {
        std::cout << value << '\n';
    }

    char stopper;
    std::cin >> stopper;
    return 0;
}
