#include <random>

uint8_t random_byte() {
    std::random_device rd{};
    std::mt19937 gen(rd());
    return gen();
}
