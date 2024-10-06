#include <iostream>
#include <string>
#include <cstdint>
#include <utility>

#include <resmanager/resmanager.hpp>

// If you have some smart pointer type...
template<typename T>
struct Bar {
    T data {};
};

// And maybe a function like this...
template<typename T, typename... Args>
Bar<T> some_function_that_constructs_a_resource(Args&&...) {
    // ...

    return {};
}

// You can specialize the loader with your own pointer type
template<typename T>
struct resmanager::Loader<T, Bar<T>> {
    using ResourcePointerType = Bar<T>;

    template<typename... Args>
    ResourcePointerType operator()(Args&&... args) const {
        return some_function_that_constructs_a_resource<T>(std::forward<Args>(args)...);
    }
};

// Some resource
struct Image {
    unsigned int w {0};
    unsigned int h {0};
    unsigned char* data {nullptr};

    // ... RAII
};

int main() {
    using namespace resmanager::literals;

    {
        resmanager::Cache<int, resmanager::Loader<int, Bar<int>>> cache;
        const auto foo {cache.load("foo"_H)};
        std::cout << foo.data << '\n';
    }

    // Default constructor, zero
    [[maybe_unused]] constexpr std::uint32_t hash0 {resmanager::HashedStr32()};

    // Compile time hashes
    constexpr std::uint64_t hash1 {resmanager::HashedStr64("s")};
    constexpr std::uint32_t hash2 {resmanager::HashedStr32("b")};

    std::cout << hash1 << '\n';
    std::cout << hash2 << '\n';

    // Compile time hashes with string literals
    [[maybe_unused]] const std::uint32_t hash3 {"Hello, Simon!"_h};
    [[maybe_unused]] const std::uint64_t hash4 {"Hello, Simon!"_H};

    // Run time hash
    [[maybe_unused]] const std::uint32_t hash5 {resmanager::HashedStr32(std::string("Hello, world!"))};

    // 64-bit key cache
    {
        resmanager::Cache<Image> cache;

        // `foo` created here
        const auto foo {cache.load("foo"_H)};
        foo->w = 400;
        std::cout << foo->w << '\n';

        // `foo` only referenced here
        const auto foo2 {cache.load("foo"_H)};
        std::cout << foo2->w << '\n';
    }

    // 32-bit key cache
    {
        resmanager::Cache<Image, resmanager::DefaultLoader<Image>, resmanager::HashedStr32> cache;

        // `foo` created here
        const auto foo {cache.load("foo"_h)};

        // `bar` created here
        const auto bar {cache.load("bar"_h)};
        bar->w = 800;
        std::cout << bar->w << '\n';

        // `bar` only referenced here
        const auto bar2 {cache.load("bar"_h)};
        std::cout << bar2->w << '\n';

        // `foo` deleted here; local reference is still valid
        cache.release("foo"_h);
    }
}
