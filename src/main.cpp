#include <iostream>
#include <vector>

auto main() -> int {
    const int size = 5;
    std::vector<int> arr(size);
    const int loop = 15;
    for (int i = 0; i < loop; i++) {
        std::cout << arr.size() << "\n";
        arr.push_back(i);
    }
    return 0;
}
