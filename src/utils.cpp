#include "utils.h"
#include <random>

int utils::get_random_int(int start, int end) {
    if (start > end) return 0;
    if (start == end) return start;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(start, end);
    return dis(gen);
}