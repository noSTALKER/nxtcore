#include <array>
#include <iostream>
#include "../include/List.h"
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
    list.insert(list.end(), {20, 30, 40});

    for (auto value : list) {
        std::cout << value << '\n';
    }

    nxt::core::Vector<int> vector;
    vector.pushBack(0);
    vector.pushBack(1);
    vector.pushBack(2);
    vector.pushBack(3);


	std::cout << "vector :\n"; 
	for (auto value : vector) {
        std::cout << value << '\n';
    }

    return 0;
}
