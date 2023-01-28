#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

// Implements the Fowler-Noll-Vo hash algorithm, or more exactly FNV-1a

namespace resmanager {
    namespace internal {
        struct Variant32 {
            static constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;
            static constexpr uint32_t FNV_PRIME = 16777619u;
        };

        struct Variant64 {
            static constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037u;
            static constexpr uint64_t FNV_PRIME = 1099511628211u;
        };

        template<typename T, typename H>
        static constexpr T fnv1a(const char* string) {
            T hash = H::FNV_OFFSET_BASIS;

            for (size_t i = 0; string[i] != '\0'; i++) {
                hash ^= static_cast<uint8_t>(string[i]);
                hash *= H::FNV_PRIME;
            }

            return hash;
        }
    }

    template<typename T, typename V>
    class HashedStr {
    public:
        constexpr HashedStr();
        explicit constexpr HashedStr(const char* string);  // TODO should be consteval, but we don't have C++20 yet
        explicit HashedStr(const std::string& string);

        constexpr operator T() const { return hash; }
        constexpr bool operator==(const HashedStr other) const;
    private:
        const T hash;
    };

    template<typename T, typename V>
    constexpr HashedStr<T, V>::HashedStr()
        : hash(0) {}

    template<typename T, typename V>
    constexpr HashedStr<T, V>::HashedStr(const char* string)
        : hash(internal::fnv1a<T, V>(string)) {}

    template<typename T, typename V>
    inline HashedStr<T, V>::HashedStr(const std::string& string)
        : hash(internal::fnv1a<T, V>(string.c_str())) {}

    template<typename T, typename V>
    constexpr bool HashedStr<T, V>::operator==(const HashedStr other) const {
        return hash == other.hash;
    }

    using HashedStr32 = HashedStr<uint32_t, internal::Variant32>;
    using HashedStr64 = HashedStr<uint64_t, internal::Variant64>;

    namespace literals {
        constexpr HashedStr32 operator""_h(const char* string, size_t) {
            return HashedStr32 {string};
        }

        constexpr HashedStr64 operator""_H(const char* string, size_t) {
            return HashedStr64 {string};
        }
    }

    /* FIXME
        might be wrong casting uint32_t to size_t (64 bits)
        IT IS WRONG casting uint64_t when size_t is 32 bits in size
    */
    template<typename H>
    struct Hash {
        constexpr size_t operator()(H hashed_string) const {
            return static_cast<size_t>(hashed_string);
        }
    };
}
