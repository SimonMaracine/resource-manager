#include <iostream>

#include "../src/resmananger/resmananger.h"

struct Foo {
    int x = 0;
};

int main() {
    const uint32_t hash = resmanager::hashed_str {"b"};

    using namespace resmanager::literals;
    const uint32_t hash2 = "Hello, Simon!"_h;

    const uint32_t hash3 = resmanager::hashed_str {std::string {"Hello, world!"}};

    resmananger::Cache<Foo> cache;

    auto foo = cache.load("foo"_h);
    foo->x = 20;

    std::cout << foo->x << std::endl;
}
