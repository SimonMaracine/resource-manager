#include <iostream>

#include "../src/resmanager/resmanager.h"

struct Foo {
    int x = 0;
};

int main() {
    constexpr uint64_t hash0 = resmanager::HashedStr64 {"s"};

    constexpr uint32_t hash = resmanager::HashedStr32 {};
    constexpr uint32_t hash1 = resmanager::HashedStr32 {"b"};

    std::cout << hash1 << std::endl;

    using namespace resmanager::literals;
    const uint32_t hash2 = "Hello, Simon!"_h;

    const uint32_t hash3 = resmanager::HashedStr32 {std::string {"Hello, world!"}};

    {
        resmanager::Cache<Foo> cache;

        auto foo = cache.load("foo"_h);
        foo->x = 20;

        std::cout << foo->x << std::endl;
    }

    {
        resmanager::Cache<Foo, resmanager::Loader<Foo>, resmanager::HashedStr64> cache;
        auto foo = cache.load("foo"_H);
        foo->x = 40;

        std::cout << foo->x << std::endl;
    }
}
